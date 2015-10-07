#include "CompileConfig.h"
#ifdef NE_INCLUDE_LIBPNG

#include "CImageLoader_PNG.h"
#include "CImage.h"

#include "png.h"


namespace novaengine
{
void read_file(png_structp png_ptr, png_bytep data, png_size_t length)
{
    io::IFile* File = (io::IFile*)png_get_io_ptr(png_ptr);
    File->read(data,length);
}

CImageLoader_PNG::CImageLoader_PNG(io::IFileSystem* fs)
{
    FileSystem = fs;
}
//-----------------------------------------------------------------------------------------------
CImageLoader_PNG::~CImageLoader_PNG()
{
    //dtor
}
//-----------------------------------------------------------------------------------------------
bool CImageLoader_PNG::isSupported(const char* file_extension)
{
    if(strcmp(file_extension,"png") == 0)
        return 1;
    return 0;
}
//-----------------------------------------------------------------------------------------------
bool CImageLoader_PNG::isSupported(io::IFile* file)
{
    png_byte header[8];
    file->read(header,sizeof(char) * 8);
    file->seek(file->getLastPos());
    if(png_sig_cmp(header, 0, 8))
        return false;
    return true;
}
//-----------------------------------------------------------------------------------------------
renderer::IImage* CImageLoader_PNG::loadImage(const char* path)
{
    io::IFile* png_file = FileSystem->open_file(path);

    if(!png_file)
        return NULL;

    renderer::IImage* image = loadImage(png_file);

    png_file->release();

    if(!image)
        return NULL;

    return image;
}
//-----------------------------------------------------------------------------------------------
renderer::IImage* CImageLoader_PNG::loadImage(io::IFile* file)
{
    u32 FileStart = file->getPos();

    renderer::IImage* png  = LoadPNG(file);

    file->seek(FileStart);

    return png;
}
//-----------------------------------------------------------------------------------------------
renderer::IImage* CImageLoader_PNG::LoadPNG(io::IFile* file)
{

    png_byte png_file_header[8];

    file->read(png_file_header,sizeof(char) * 8);

    int is_png = !png_sig_cmp(png_file_header,0,8);
    if(!is_png)
    {
        LOG_ERROR("Failed to load image - corrupted data[not png]\n");
        return NULL;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        LOG_ERROR("Failed to load image - libpng internal error\n");
        return NULL;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        LOG_ERROR("Failed to load image - libpng internal error\n");
        png_destroy_read_struct(&png_ptr,(png_infopp)NULL,(png_infopp)NULL);
        return NULL;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if(!end_info)
    {
        LOG_ERROR("Failed to load image - libpng internal error\n");
        png_destroy_read_struct(&png_ptr,&end_info,(png_infopp)NULL);
        return NULL;
    }
    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return NULL;
    }

    // init png reading with function read_file insted standart io like fread(void*,size_t,size_t,FILE* f);
    png_set_read_fn(png_ptr, (void*)file, read_file);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    s32 bit_depth, color_type;
    png_uint_32 width, height;
//    u32 byte_depth;

    png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,NULL,NULL,NULL);

    //!Converting png
    //------------------------------------------------------------------------------------------
    // Convert high bit colors to 8 bit colors
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);

    //convert low bit colors to 8 bit color
    //if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    //    png_set_gray_1_2_4_to_8(png_ptr);

    //Convert palette color to true color
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    // Convert gray color to true color
    if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    png_read_update_info(png_ptr, info_ptr);
    //------------------------------------------------------------------------------------------

    png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,NULL,NULL,NULL);

//    png_byte channels = png_get_channels(png_ptr, info_ptr);
    png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    png_byte* data = new png_byte[row_bytes * height];

    png_byte **row_pointers = new png_byte * [height];

    for (unsigned int i = 0; i < height; i++)
        row_pointers[height - i - 1] = data + i * row_bytes;

    png_read_image(png_ptr, row_pointers);

    png_read_end(png_ptr, 0);

    delete [] row_pointers;

    png_destroy_read_struct(&png_ptr, 0, 0);

    renderer::E_PIXEL_FORMAT PixelFomat;
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    {
        PixelFomat = renderer::EPF_R8G8B8A8;
    }
    else
    {
        PixelFomat = renderer::EPF_R8G8B8;
    }

    //CColorConverter::flip_color_array(data,width,height,byte_depth,0,false,false);
    renderer::CImage* img =  new renderer::CImage(PixelFomat,core::dim2u(width,height),data);
    delete[] data;
    return img;
}
//-----------------------------------------------------------------------------------------------
}

#endif // INCLUDE_LIBPNG
