#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char *text_to_write = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. At ultrices mi tempus imperdiet nulla malesuada pellentesque elit. Non enim praesent elementum facilisis leo vel fringilla est ullamcorper. Non quam lacus suspendisse faucibus interdum. Metus vulputate eu scelerisque felis imperdiet. At ultrices mi tempus imperdiet nulla malesuada pellentesque. At tempor commodo ullamcorper a lacus vestibulum. Consequat semper viverra nam libero justo laoreet sit amet. Facilisis magna etiam tempor orci eu. Convallis tellus id interdum velit laoreet id donec ultrices. Interdum velit euismod in pellentesque massa placerat duis ultricies. Senectus et netus et malesuada fames. Pharetra vel turpis nunc eget lorem dolor. Nisi porta lorem mollis aliquam ut porttitor leo a. Euismod elementum nisi quis eleifend quam adipiscing vitae proin.";
    char *file_name = "output.txt";
    unsigned int length = strlen(text_to_write);
    unsigned int first_half, second_half, result;
    off_t file_offset;
    first_half = length / 2;
    second_half = length - first_half;
    char *first_array = calloc(first_half + 1, sizeof(char));
    char *second_array = calloc(second_half + 1, sizeof(char));
    if (first_array == NULL || second_array == NULL)
    {
        perror("\ncalloc failure!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(first_array, text_to_write, first_half);
    first_array[first_half] = '\0';
    memcpy(second_array, &text_to_write[first_half], second_half);
    second_array[second_half] = '\0';
    printf("\n[parent] half\n\n");
    printf("%s", first_array);
    printf("\n\n[child] half\n\n");
    printf("%s", second_array);
    int fd = open(file_name,
                  O_CREAT | O_TRUNC | O_WRONLY,
                  S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    switch (fork())
    {
    case 0:
        file_offset = lseek(fd, first_half, SEEK_SET);
        if (file_offset == -1)
        {
            perror("lseek()");
            exit(EXIT_FAILURE);
        }
        result = write(fd, second_array, second_half);
        if (result == -1)
        {
            perror("write()");
            exit(EXIT_FAILURE);
        }
        exit(0);

        break;
    case -1:
        perror("\nfork() error!\n");
        exit(EXIT_FAILURE);

        break;
    default:
        file_offset = lseek(fd, 0, SEEK_SET);
        if (file_offset == -1)
        {
            perror("lseek()");
            exit(EXIT_FAILURE);
        }
        result = write(fd, first_array, first_half);
        if (result == -1)
        {
            perror("write()");
            exit(EXIT_FAILURE);
        }
        if (wait(NULL) == -1)
        {
            perror("wait()");
            exit(EXIT_FAILURE);
        }

        break;
    }
    if (close(fd) == -1)
    {
        perror("close()");
    }
    free(first_array);
    free(second_array);

    return 0;
}
