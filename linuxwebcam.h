#ifndef _LINUXWEBCAM_H_
#define _LINUXWEBCAM_H_



/*
 *  Linux Webcamera Class
 *
 *  Author:         Gavin Lobo
 *  Website:        www.rabidgeek.net
 *  Github:         https://github.com/rabidgeek
 *
 *  Description:    This class file was created in hope that it would be easier to capture webcamera data in linux.
 *                  It is heavily based on the capture.c example provided by video4linux.  The capture.c code was
 *                  encapsulated into a C++ class file.
 *
 *                  To use this, simply inherit from the linuxWebcam class and override the processImage(imageData * img)
 *                  method.  See example.cpp
 *
 *                  I have only tested this on my plug-n-play Creative Live! Cam Chat HD Hi-Speed USB.
 *                  Testing on other cameras would be nice.
 *
 *
 *                  Things I might want to add:  ARGB converstion
 *
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

#include "bitmapImage.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define CLAMP(x,min,max) ( x < min ? min : ( x > max  ? max : x))

/* RGB colour structure */
typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} colorRGB;

typedef enum {
    NONE,
    YUV,
    RGB,
    ARGB
} IMAGEFORMAT;

/*
    A structure for holding image information.

    data points to the first memory array
    size refers to the number of BYTES in the array - This will usually mean that it's 3x(# of of total pixels), each pixel has 1 R, 1 G, and 1 B value
*/
class imageData
{
    public:
        uint8_t *data;              // pointer to the image data.  DO NOT FREE THIS DATA;

        imageData()
        {
            data        = NULL;
            size        = 0;
            format      = NONE;
            width       = 0;
            height      = 0;
        }


        int getSize() { return size; }
        int getWidth() { return width;}
        int getHeight() { return height;}
        int getFormat() { return format; }

        // Do not use these functions
        void setData(uint8_t * externalData, int SIZE, int w, int h, IMAGEFORMAT imgFormat)
        {
            data        = externalData;
            size        = SIZE;
            format      = imgFormat;
            width       = w;
            height      = h;
        }

        void allocateSpace(int WIDTH, int HEIGHT, IMAGEFORMAT imgFormat)
        {
            if( data != NULL)
                delete [] data;

            switch( imgFormat )
            {
                case YUV:
                    size = WIDTH*HEIGHT*2;
                    break;
                case RGB:
                    size = WIDTH*HEIGHT*3;
                    break;
                default:
                    size = 0;
            }
            data        = new uint8_t[size];
            width       = WIDTH;
            height      = HEIGHT;
        }

    private:
        int size;                   // number of BYTES in the array
        int byteOrder;              // the type of image, either BYTEORDER_YUYV or BYTEORDER_RGBRGB
        int width;                  // Width of the image
        int height;                 // Height of the image
        IMAGEFORMAT format;                // image format: YUYV, or RGBRGB

};




class linuxWebcam
{
public:


private:
    struct buffer
    {
        void *                  start;
        size_t                  length;
    };



    char *           dev_name;
    int     	     io;
    int              fd;
    buffer *         buffers;
    unsigned int     n_buffers;

    int             imageWidth;
    int             imageHeight;

    IMAGEFORMAT     imageFormat;

    imageData       RGBDATA;

public:

    static const int IO_METHOD_MMAP         = 0;
    static const int IO_METHOD_READ         = 1;
    static const int IO_METHOD_USERPTR      = 2;

    static const int BYTEORDER_YUYV         = 0;
    static const int BYTEORDER_RGBRGB       = 1;

    linuxWebcam()
    {

        io                      = IO_METHOD_MMAP;
        fd                      = -1;
        buffers                 = NULL;
        n_buffers               = 0;
    }

    int getWidth(){ return imageWidth;}
    int getHeight(){ return imageHeight;}

    void setImageFormat(IMAGEFORMAT format)
    {
        imageFormat = format;
    }

    IMAGEFORMAT getImageFormat()
    {
        return imageFormat;
    }
    /*
    Converts YUV values into RGB
        Converts 4 bytes of YUV data into 6 bytes of RGB data.

        Requires a reference to the 4 bytes of YUV data and a reference to the RGB data.

        (input)  YUV byte order: Y1,U,Y2,V
        (output) RGB byte order: R1,G1,B1,R2,G2,B2
    */
    static void YUYV_to_RGBRGB(uint8_t *YUYV_data, uint8_t *RGBRGB_data)
    {
        /* These temprorary interger variables are used to do the conversion.  If you use char you will get an
           overflow and the colours will look incorrect, Try it! */
        int              r1,g1,b1,r2,g2,b2;

        r1 = 1.4075 * (YUYV_data[3] - 128);
        g1 = 0.3455 * (YUYV_data[1] - 128) - (0.7169 * (YUYV_data[3] - 128));
        b1 = 1.7790 * (YUYV_data[1] - 128);

        r2 = r1 + YUYV_data[2];
        g2 = g1 + YUYV_data[2];
        b2 = b1 + YUYV_data[2];

        r1 += YUYV_data[2];
        g1 += YUYV_data[2];
        b1 += YUYV_data[2];

        RGBRGB_data[0] = CLAMP(r1,0,255);
        RGBRGB_data[1] = CLAMP(g1,0,255);
        RGBRGB_data[2] = CLAMP(b1,0,255);
        RGBRGB_data[3] = CLAMP(r2,0,255);
        RGBRGB_data[4] = CLAMP(g2,0,255);
        RGBRGB_data[5] = CLAMP(b2,0,255);
    }

    /*
    Converts YUV values into RGB
        Converts 3 bytes of YUV data into 3 bytes or RGB data

         input: Y, U and V data, in 8 bytes

        output: RGB structure

    */
    static colorRGB YUV_to_RGB( uint8_t y, uint8_t u, uint8_t v)
    {

        /* These temprorary interger variables are used to do the conversion.  If you use char you will get an
           overflow and the colours will look incorrect, Try it! */
        int              r,g,b;
        //unsigned char  r,g,b;
        //char           r,g,b;

        colorRGB bm;

        r = y + 1.4075 * (v - 128);
        g = y - 0.3455 * (u - 128) - (0.7169 * (v - 128));
        b = y + 1.7790 * (u - 128);

        bm.r = CLAMP(r,0,255);
        bm.g = CLAMP(g,0,255);
        bm.b = CLAMP(b,0,255);

        return(bm);
    }

    /*
    Converts an entire array of YUV422 data into RGB888 data
        Converts 3 bytes of YUV data into 3 bytes or RGB data

         input: pointer to the first element of the array of YUV data.  The YUV data must be in the following format: Y1,U,Y2,V,Y3,U1,Y4,V1, .....

         output: imageData structure that contains the RGB data

    */

    /*
    imageData YUV422toRGB(void * p, int size)
    {
        printf("Converting to RGB\n");

        unsigned char Y0,Y1,U,V;
        uint8_t * pix = (uint8_t*)p;

        int rgb_counter = 0;

        imageData img;
        img.size = size * 3 / 2;
        img.data = new uint8_t[ img.size ];  // data that will hold (mageWidth*imageHeight*3 bytes of data, 3 bytes for each pixel

        for(int i=0; i<size; i+=4)
        {
            //Convert 4 bytes of YUV data into 6 bytes of RGB data
            YUYV_to_RGBRGB(&pix[i], &img.data[rgb_counter]);
            rgb_counter += 6;
        }
        printf("Image conversion complete\n");

        return(img);
    }
    */


    void YUV422toRGB(imageData * inputImage, imageData * outputImage)
    {
        //printf("Converting to RGB\n");

        int rgb_counter = 0;

        // This assumes that the outputImage data has already been created
        outputImage->setData(       outputImage->data,                                          // Set th
                                    inputImage->getSize() * 3 /2,
                                    outputImage->getWidth(),
                                    outputImage->getHeight(),
                                    RGB);

        int size = inputImage->getSize();
        for(int i=0; i<size; i+=4)
        {
            //Convert 4 bytes of YUV data into 6 bytes of RGB data
            YUYV_to_RGBRGB(&inputImage->data[i], &outputImage->data[rgb_counter]);
            rgb_counter += 6;
        }
        //printf("Image conversion complete\n");

    }

    virtual void processImage(imageData * img)
    {
        printf("=========================================================\n");
        printf(" DEFAULT PROCESSIMAGE()\n");
        printf("=========================================================\n");
        printf(" Image Recieved\n");
        printf("        Image Size:             %d x %d\n",img->getWidth(), img->getHeight());
        switch( img->getFormat())
        {
            case RGB:
                printf("        Image Format:           RGB\n"   );
                break;
            case YUV:
                printf("        Image Format:           YUV\n"   );
                break;
            default:
                break;
        };

        printf("        Data Size:              %d bytes\n", img->getSize() );

        printf("=========================================================\n");

        return;
    }



    /* ==============================================================================================================
    Most of the methods written below come from the capture.c sample.  I am not sure what most of the functions do
    If someone would like to comment them further, that would be great.
    ================================================================================================================*/
    void
    errno_exit                      (const char *           s)
    {
        fprintf (stderr, "%s error %d, %s\n",s, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    int
    xioctl                          (int                    fd,
                                     int                    request,
                                     void *                 arg)
    {
        int r;

        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);

        return r;
    }

    int
    read_frame			(void)
    {
        struct v4l2_buffer buf;
        unsigned int i;

        imageData img;

        switch (io)
        {
            case IO_METHOD_READ:
                if (-1 == read (fd, buffers[0].start, buffers[0].length))
                {
                    switch (errno)
                    {
                    case EAGAIN:
                        return 0;

                    case EIO:
                        /* Could ignore EIO, see spec. */

                        /* fall through */

                    default:
                        errno_exit ("read");
                    }
                }

                img.setData((uint8_t*)buffers[0].start, (int)buffers[0].length, imageWidth, imageHeight,YUV);

                switch (imageFormat)
                {
                    case RGB:
                        YUV422toRGB(&img, &RGBDATA);
                        processImage(&RGBDATA);
                        break;
                    case YUV:
                    default:
                        processImage(&img);
                }

            case IO_METHOD_MMAP:

                CLEAR (buf);

                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;

                if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf))
                {
                    switch (errno)
                    {
                    case EAGAIN:
                        return 0;

                    case EIO:
                        /* Could ignore EIO, see spec. */

                        /* fall through */

                    default:
                        errno_exit ("VIDIOC_DQBUF");
                    }
                }



                assert (buf.index < n_buffers);

                /*
                printf("\nbuffer inded = %d\n", (int)buf.index);
                printf("Image size: %d x %d = %d pixels total\n", imageWidth,imageHeight);
                printf("Total Pixel:    %d\n", imageWidth*imageHeight);
                printf("YUV Data size:  %d\n", imageWidth*imageHeight*2);
                printf("  Data size:    %d\n", (int)buffers[buf.index].length);
                printf("length = %d  %d\n", (int)buffers[buf.index].length, imageWidth*imageHeight*2);
                */

                img.setData((uint8_t*)buffers[buf.index].start, (int)buffers[buf.index].length, imageWidth, imageHeight,YUV);

                switch (imageFormat)
                {
                    case RGB:
                        YUV422toRGB(&img, &RGBDATA);
                        processImage(&RGBDATA);
                        break;
                    case YUV:
                    default:
                        processImage(&img);
                }

                if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                    errno_exit ("VIDIOC_QBUF");

                break;


            case IO_METHOD_USERPTR:
                CLEAR (buf);

                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_USERPTR;

                if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf))
                {
                    switch (errno)
                    {
                    case EAGAIN:
                        return 0;

                    case EIO:
                        /* Could ignore EIO, see spec. */
                        /* fall through */

                    default:
                        errno_exit ("VIDIOC_DQBUF");
                    }
                }

                for (i = 0; i < n_buffers; ++i)
                    if (buf.m.userptr == (unsigned long) buffers[i].start
                            && buf.length == buffers[i].length)
                        break;

                assert (i < n_buffers);

                //==========================
                img.setData((uint8_t*)buf.m.userptr, (int)buf.length, imageWidth, imageHeight,YUV);

                switch (imageFormat)
                {
                    case RGB:
                        YUV422toRGB(&img, &RGBDATA);
                        processImage(&RGBDATA);
                        break;
                    case YUV:
                    default:
                        processImage(&img);
                }
                //==========================


                if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                    errno_exit ("VIDIOC_QBUF");

                break;
        }

        return 1;
    }


    /* Capture a frame from the webcam and then call the process image method
    */
    void captureFrameRGB(void)
    {
        imageFormat = RGB;
        for (;;)
        {
            if (read_frame ())
                break;
            /* EAGAIN - continue select loop. */
        }
    }

    void captureFrameYUV(void)
    {
        imageFormat = YUV;
        for (;;)
        {
            if (read_frame ())
                break;
            /* EAGAIN - continue select loop. */
        }
    }


    void stop_capturing(void)
    {
        enum v4l2_buf_type type;

        switch (io)
        {
        case IO_METHOD_READ:
            /* Nothing to do. */
            break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
            type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            if (-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
                errno_exit ("VIDIOC_STREAMOFF");

            break;
        }
    }

    void
    start_capturing                 (void)
    {
        unsigned int i;
        enum v4l2_buf_type type;

        switch (io)
        {
        case IO_METHOD_READ:
            /* Nothing to do. */
            break;

        case IO_METHOD_MMAP:
            for (i = 0; i < n_buffers; ++i)
            {
                struct v4l2_buffer buf;

                CLEAR (buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = i;

                if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                    errno_exit ("VIDIOC_QBUF");
            }

            type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
                errno_exit ("VIDIOC_STREAMON");

            break;

        }
    }

    void
    uninitDevice                   (void)
    {
        unsigned int i;

        switch (io)
        {
        case IO_METHOD_MMAP:
            for (i = 0; i < n_buffers; ++i)
                if (-1 == munmap (buffers[i].start, buffers[i].length))
                    errno_exit ("munmap");
            break;
        }

        free (buffers);
    }

    void
    init_read			(unsigned int		buffer_size)
    {
        buffers = (buffer*)calloc (1, sizeof (*buffers));

        if (!buffers)
        {
            fprintf (stderr, "Out of memory\n");
            exit (EXIT_FAILURE);
        }

        buffers[0].length = buffer_size;
        buffers[0].start = malloc (buffer_size);

        if (!buffers[0].start)
        {
            fprintf (stderr, "Out of memory\n");
            exit (EXIT_FAILURE);
        }
    }

    void
    init_mmap			(void)
    {
        struct v4l2_requestbuffers req;

        CLEAR (req);

        req.count               = 10;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_MMAP;

        if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req))
        {
            if (EINVAL == errno)
            {
                fprintf (stderr, "%s does not support "
                         "memory mapping\n", dev_name);
                exit (EXIT_FAILURE);
            }
            else
            {
                errno_exit ("VIDIOC_REQBUFS");
            }
        }

        if (req.count < 2)
        {
            fprintf (stderr, "Insufficient buffer memory on %s\n",
                     dev_name);
            exit (EXIT_FAILURE);
        }

        buffers = (buffer*)calloc (req.count, sizeof (*buffers));

        if (!buffers)
        {
            fprintf (stderr, "Out of memory\n");
            exit (EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
        {
            struct v4l2_buffer buf;

            CLEAR (buf);

            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = n_buffers;

            if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
                errno_exit ("VIDIOC_QUERYBUF");

            buffers[n_buffers].length = buf.length;
            buffers[n_buffers].start =
                mmap (NULL /* start anywhere */,
                      buf.length,
                      PROT_READ | PROT_WRITE /* required */,
                      MAP_SHARED /* recommended */,
                      fd, buf.m.offset);

            if (MAP_FAILED == buffers[n_buffers].start)
                errno_exit ("mmap");
        }
    }

    void
    init_userp			(unsigned int		buffer_size)
    {
        struct v4l2_requestbuffers req;
        unsigned int page_size;

        page_size = getpagesize ();
        buffer_size = (buffer_size + page_size - 1) & ~(page_size - 1);

        CLEAR (req);

        req.count               = 4;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req))
        {
            if (EINVAL == errno)
            {
                fprintf (stderr, "%s does not support "
                         "user pointer i/o\n", dev_name);
                exit (EXIT_FAILURE);
            }
            else
            {
                errno_exit ("VIDIOC_REQBUFS");
            }
        }

        buffers = (buffer*)calloc (4, sizeof (*buffers));

        if (!buffers)
        {
            fprintf (stderr, "Out of memory\n");
            exit (EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < 4; ++n_buffers)
        {
            buffers[n_buffers].length = buffer_size;
            buffers[n_buffers].start = memalign (/* boundary */ page_size,
                                       buffer_size);

            if (!buffers[n_buffers].start)
            {
                fprintf (stderr, "Out of memory\n");
                exit (EXIT_FAILURE);
            }
        }
    }

    void
    initDevice                     (int pixelWidth, int pixelHeight)
    {


        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;

        unsigned int min;

        if (-1 == xioctl (fd, VIDIOC_QUERYCAP, &cap))
        {
            if (EINVAL == errno)
            {
                fprintf (stderr, "%s is no V4L2 device\n", dev_name);
                exit (EXIT_FAILURE);
            }
            else
            {
                errno_exit ("VIDIOC_QUERYCAP");
            }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
        {
            fprintf (stderr, "%s is no video capture device\n", dev_name);
            exit (EXIT_FAILURE);
        }

        switch (io)
        {

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
            if (!(cap.capabilities & V4L2_CAP_STREAMING))
            {
                fprintf (stderr, "%s does not support streaming i/o\n",
                         dev_name);
                exit (EXIT_FAILURE);
            }

            break;

        }


        /* Select video input, video standard and tune here.*/
        CLEAR (cropcap);

        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (0 == xioctl (fd, VIDIOC_CROPCAP, &cropcap))
        {
            crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            crop.c = cropcap.defrect; /* reset to default */

            if (-1 == xioctl (fd, VIDIOC_S_CROP, &crop))
            {
                switch (errno)
                {
                case EINVAL:
                    /* Cropping not supported. */
                    break;
                default:
                    /* Errors ignored. */
                    break;
                }
            }
        }
        else
        {
            /* Errors ignored. */
        }


        CLEAR (fmt);

        fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width       = pixelWidth;
        fmt.fmt.pix.height      = pixelHeight;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

        if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
            errno_exit ("VIDIOC_S_FMT");

        /* Note VIDIOC_S_FMT may change width and height. */

        /* Buggy driver paranoia. */
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
            fmt.fmt.pix.bytesperline = min;

        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
            fmt.fmt.pix.sizeimage = min;

        imageWidth = fmt.fmt.pix.width;
        imageHeight = fmt.fmt.pix.height;

        //printf("Device Initialized: %d %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);

        switch (io)
        {
        case IO_METHOD_READ:
            init_read (fmt.fmt.pix.sizeimage);
            break;

        case IO_METHOD_MMAP:
            init_mmap ();
            break;

        case IO_METHOD_USERPTR:
            init_userp (fmt.fmt.pix.sizeimage);
            break;
        }

        RGBDATA.allocateSpace(pixelWidth, pixelHeight,RGB);
    };

    void
    close_device                    (void)
    {
        if (-1 == close (fd))
            errno_exit ("close");

        fd = -1;
    };

    void
    open_device                     (char deviceName[])
    {
        dev_name = new char[strlen(deviceName)+2];
        strcpy(dev_name,deviceName);

        struct stat st;

        if (-1 == stat (dev_name, &st))
        {
            fprintf (stderr, "Cannot identify '%s': %d, %s\n", dev_name, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }

        if (!S_ISCHR (st.st_mode))
        {
            fprintf (stderr, "%s is no device\n", dev_name);
            exit (EXIT_FAILURE);
        }

        fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd)
        {
            fprintf (stderr, "Cannot open '%s': %d, %s\n", dev_name, errno, strerror (errno));
            exit (EXIT_FAILURE);
        }
    };


};

#endif
