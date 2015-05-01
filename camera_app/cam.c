/*
 * Galileo Benchmark
 *
 * Description: This program controls the webcam connected to the CPU. 
 *              A couple of samples of images are captured and processed
 *              to save them as ppm images
 *
 * author:
 *  Poorvaja Rajagopal
 *
 * Version: 1.0
 * Date: 5/1/2015
 *
 *
 * Version: 1.0
 * Date: 5/1/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <libv4l2.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>


#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
        void   *start;
        size_t length;
};

static void xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = v4l2_ioctl(fh, request, arg);
        } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

        if (r == -1) {
                fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
}
static inline uint64_t RDTSC()
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

int main(int argc, char **argv)
{
        struct v4l2_format              fmt;
        struct v4l2_buffer              buf;
        struct v4l2_requestbuffers      req;
        enum v4l2_buf_type              type;
        fd_set                          fds;
        struct timeval                  tv;
        int                             r, fd = -1;
        unsigned int                    i, n_buffers;
        char                            *dev_name = "/dev/video0";
        char                            out_name[256];
        FILE                            *fout=0;
        struct buffer                   *buffers=0;
        uint64_t starti=0, endi=0;
        struct timeval begini, finishi;
        starti = RDTSC();
        gettimeofday(&begini, NULL);
        fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);
        if (fd < 0) {
                perror("Cannot open device");
                exit(EXIT_FAILURE);
        }

        CLEAR(fmt);
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width       = 640;
        fmt.fmt.pix.height      = 480;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
        xioctl(fd, VIDIOC_S_FMT, &fmt);
        if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) {
                printf("Libv4l didn't accept RGB24 format. Can't proceed.\n");
                exit(EXIT_FAILURE);
        }
        if ((fmt.fmt.pix.width != 640) || (fmt.fmt.pix.height != 480))
                printf("Warning: driver is sending image at %dx%d\n",
                        fmt.fmt.pix.width, fmt.fmt.pix.height);

        CLEAR(req);
        req.count = 2;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;
        xioctl(fd, VIDIOC_REQBUFS, &req);

        buffers = calloc(req.count, sizeof(*buffers));
        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                CLEAR(buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

                xioctl(fd, VIDIOC_QUERYBUF, &buf);

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
                              PROT_READ | PROT_WRITE, MAP_SHARED,
                              fd, buf.m.offset);

                if (MAP_FAILED == buffers[n_buffers].start) {
                        perror("mmap");
                        exit(EXIT_FAILURE);
                }
        }
printf("2");
        for (i = 0; i < n_buffers; ++i) {
                CLEAR(buf);
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;
                buf.index = i;
                xioctl(fd, VIDIOC_QBUF, &buf);
        }
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        xioctl(fd, VIDIOC_STREAMON, &type);
printf("3");
        for (i = 0; i < 20; i++) {
                do {
                        FD_ZERO(&fds);
                        FD_SET(fd, &fds);
                      printf("4");
                        
                        tv.tv_sec = 2;
                        tv.tv_usec = 0;

                        r = select(fd + 1, &fds, NULL, NULL, &tv);
                } while ((r == -1 && (errno = EINTR)));
              if (r == -1) {
                        perror("select");
                        return errno;
                }  

//                CLEAR(buf);
  //              buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //            buf.memory = V4L2_MEMORY_MMAP;
   //            xioctl(fd, VIDIOC_DQBUF, &buf);
int qw, fy;
//do {
for (fy=0; fy<5; fy++)
{
qw = v4l2_ioctl(fd, VIDIOC_DQBUF, &buf);
if (qw!=-1)
break;  
}
//}while(qw==-1);
                sprintf(out_name, "out%03d.ppm", i);
                fout = fopen(out_name, "w");
                if (!fout) {
                        perror("Cannot open image");
                        exit(EXIT_FAILURE);
                }
                fprintf(fout, "P6\n%d %d 255\n",
                        fmt.fmt.pix.width, fmt.fmt.pix.height);
                fwrite(buffers[buf.index].start, buf.bytesused, 1, fout);
                fclose(fout);

            //    xioctl(fd, VIDIOC_QBUF, &buf);
for (fy=0; fy<5; fy++)
{
  qw = v4l2_ioctl(fd, VIDIOC_QBUF, &buf);
if (qw!=-1)
break;
}

        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        xioctl(fd, VIDIOC_STREAMOFF, &type);
//qw = v4l2_ioctl(fd, VIDIOC_STREAMOFF, &type);
        for (i = 0; i < n_buffers; ++i)
                v4l2_munmap(buffers[i].start, buffers[i].length);
        v4l2_close(fd);
        endi=RDTSC();
        gettimeofday(&finishi, NULL);
   printf("time elapesed:%ld\n", ((finishi.tv_sec * 1000000 + finishi.tv_usec) - (begini.tv_sec * 1000000 + begini.tv_usec)));
   printf("cycles");
printf("%"PRIu64"\n", endi-starti);
 
        return 0;
}
