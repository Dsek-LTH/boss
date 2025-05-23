#include "choochoo.h"

#include "common.h"
#include "led-matrix.h"
#include "graphics.h"

#include <unistd.h>
#include <string.h>
#include <string>

constexpr int FRAME_RATE = 25;

constexpr int ROWS = LED_MATRIX_HEIGHT;
constexpr int COLS = BOSS_MATRIX_WIDTH * 2;

constexpr int TRAIN_OFFSET = BOSS_MATRIX_WIDTH;
constexpr int TRAIN_HEIGHT = 19;

constexpr int SMOKE_OFFSET = 8;
constexpr int SMOKE_HEIGHT = 5;

constexpr int WHEEL_OFFSET = 1;
constexpr int WHEEL_HEIGHT = 5;

constexpr int ANIM_LENGTH = COLS;
constexpr int ANIM_CART_OFFSET = 35;
constexpr int DEFAULT_MAX_NUM_CARTS = 3;

static int usage(const char *progname)
{
    fprintf(stderr, "usage: %s [options]\n", progname);
    fprintf(stderr, "Choochoo!\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr,
            "\t-c <num_carts>    : Number of train carts\n"
            "\t-C <r,g,b>        : Train color (default 255,255,255)\n"
            "\t-r                : Funky rainbow train (may not be used along with -C)\n\n");
    rgb_matrix::PrintMatrixFlags(stderr);
    return -1;
}

static inline int get_matrix_value(char c)
{
    return c == '@' ? 1 : 0;
}

int **calc_lok_matrix(int num_carts)
{
    int **matrix = new int *[ROWS];
    for (size_t i = 0; i < ROWS; i++)
    {
        matrix[i] = new int[COLS * num_carts * 2];
        memset(matrix[i], 0, sizeof(int) * COLS * num_carts * 2);
    }

    std::vector<const char *> lok{
        LOK18,
        // padding for wheel section
        LOK_EMPTY,
        LOK_EMPTY,
        LOK_EMPTY,
        LOK_EMPTY,
        LOK_EMPTY,
        // end padding
        LOK12,
        LOK11,
        LOK10,
        LOK9,
        LOK8,
        LOK7,
        LOK6,
        LOK5,
        LOK4,
        LOK3,
        LOK2,
        LOK1,
        LOK0};

    std::vector<const char *> m_vagn{
        MIDDLE_VAGN14,
        MIDDLE_VAGN13,
        MIDDLE_VAGN12,
        MIDDLE_VAGN11,
        MIDDLE_VAGN10,
        MIDDLE_VAGN9,
        MIDDLE_VAGN8,
        MIDDLE_VAGN7,
        MIDDLE_VAGN6,
        MIDDLE_VAGN5,
        MIDDLE_VAGN4,
        MIDDLE_VAGN3,
        MIDDLE_VAGN2,
        MIDDLE_VAGN1,
        MIDDLE_VAGN0,
        VAGN_E,
        VAGN_E,
        VAGN_E,
        VAGN_E,
    };

    std::vector<const char *> l_vagn{
        LAST_VAGN14,
        LAST_VAGN13,
        LAST_VAGN12,
        LAST_VAGN11,
        LAST_VAGN10,
        LAST_VAGN9,
        LAST_VAGN8,
        LAST_VAGN7,
        LAST_VAGN6,
        LAST_VAGN5,
        LAST_VAGN4,
        LAST_VAGN3,
        LAST_VAGN2,
        LAST_VAGN1,
        LAST_VAGN0,
        VAGN_E,
        VAGN_E,
        VAGN_E,
        VAGN_E,
    };

    std::vector<std::string> lok_w_c{};

    for (size_t i = 0; i < lok.size(); i++)
    {
        std::string s{lok[i]};
        for (int j = 0; j < num_carts - 1; j++)
        {
            s += m_vagn[i];
        }
        s += l_vagn[i];
        lok_w_c.push_back(s);
    }

    int train_len = lok_w_c[0].size();

    for (int i = 0; i < TRAIN_HEIGHT; i++)
    {
        for (int j = 0; j < train_len; j++)
        {
            matrix[ROWS - i - 1][TRAIN_OFFSET + j] = get_matrix_value(lok_w_c[i][j]);
        }
    }

    return matrix;
}

const char *get_wheel_17(int frame)
{
    return std::vector<const char *>{LOK17_0, LOK17_1, LOK17_2, LOK17_3}[frame];
}

const char *get_wheel_16(int frame)
{
    return std::vector<const char *>{LOK16_0, LOK16_1, LOK16_2, LOK16_3}[frame];
}

const char *get_wheel_15(int frame)
{
    return std::vector<const char *>{LOK15_0, LOK15_1, LOK15_2, LOK15_3}[frame];
}

const char *get_wheel_14(int frame)
{
    return std::vector<const char *>{LOK14_0, LOK14_1, LOK14_2, LOK14_3}[frame];
}

const char *get_wheel_13(int frame)
{
    return std::vector<const char *>{LOK13_0, LOK13_1, LOK13_2, LOK13_3}[frame];
}

int **calc_wheel_matrix(int frame, int num_carts)
{
    int **matrix = new int *[ROWS];
    for (size_t i = 0; i < ROWS; i++)
    {
        matrix[i] = new int[COLS * num_carts * 2];
        memset(matrix[i], 0, sizeof(int) * COLS * num_carts * 2);
    }

    std::vector<const char *> il{
        get_wheel_17(frame),
        get_wheel_16(frame),
        get_wheel_15(frame),
        get_wheel_14(frame),
        get_wheel_13(frame)};

    int train_len = strlen(LOK0);

    for (int i = WHEEL_OFFSET; i <= WHEEL_HEIGHT; i++)
    {
        for (int j = 0; j < train_len; j++)
        {
            matrix[ROWS - i - 1][TRAIN_OFFSET + j] = get_matrix_value(il[i - WHEEL_OFFSET][j]);
        }
    }

    return matrix;
}

const char *get_smoke_0(int frame)
{
    return std::vector<const char *>{SMOKE0_0, SMOKE0_1}[frame];
}

const char *get_smoke_1(int frame)
{
    return std::vector<const char *>{SMOKE1_0, SMOKE1_1}[frame];
}

const char *get_smoke_2(int frame)
{
    return std::vector<const char *>{SMOKE2_0, SMOKE2_1}[frame];
}

const char *get_smoke_3(int frame)
{
    return std::vector<const char *>{SMOKE3_0, SMOKE3_1}[frame];
}

const char *get_smoke_4(int frame)
{
    return std::vector<const char *>{SMOKE4_0, SMOKE4_1}[frame];
}

int **calc_smoke_matrix(int frame, int num_carts)
{
    int **matrix = new int *[ROWS];
    for (size_t i = 0; i < ROWS; i++)
    {
        matrix[i] = new int[COLS * num_carts * 2];
        memset(matrix[i], 0, sizeof(int) * COLS * num_carts * 2);
    }

    std::vector<const char *> il{
        get_smoke_0(frame),
        get_smoke_1(frame),
        get_smoke_2(frame),
        get_smoke_3(frame),
        get_smoke_4(frame)};

    int smoke_len = strlen(SMOKE0_0);

    for (int i = 0; i < SMOKE_HEIGHT; i++)
    {
        for (int j = 0; j < smoke_len; j++)
        {
            matrix[i + SMOKE_OFFSET][TRAIN_OFFSET + j] = get_matrix_value(il[i][j]);
        }
    }

    return matrix;
}

rgb_matrix::FrameCanvas *draw_frame(rgb_matrix::RGBMatrix *rgb_mat, rgb_matrix::FrameCanvas *offscreen_canvas, int **lok_static, int **wheels, int **smoke, const rgb_matrix::Color &color)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < BOSS_MATRIX_WIDTH; j++)
        {
            if (lok_static[i][j] || wheels[i][j] || smoke[i][j])
            {
                offscreen_canvas->SetPixel(j, i, color.r, color.g, color.b);
            }
        }
    }
    return offscreen_canvas;
}

rgb_matrix::FrameCanvas *draw_lok(rgb_matrix::RGBMatrix *rgb_mat, rgb_matrix::FrameCanvas *offscreen_canvas, int **lok_static, int **wheels, int **smoke, const rgb_matrix::Color &color)
{
    offscreen_canvas->Clear();
    offscreen_canvas = draw_frame(rgb_mat, offscreen_canvas, lok_static, wheels, smoke, color);
    return rgb_mat->SwapOnVSync(offscreen_canvas);
}

static bool parseColor(rgb_matrix::Color *c, const char *str)
{
    return sscanf(str, "%hhu,%hhu,%hhu", &c->r, &c->g, &c->b) == 3;
}

int main(int argc, char **argv)
{
    srand(time(nullptr));

    rgb_matrix::RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;
    matrix_options.hardware_mapping = HW_ID;
    matrix_options.rows = LED_MATRIX_HEIGHT;
    matrix_options.cols = LED_MATRIX_WIDTH;
    matrix_options.chain_length = BOSS_WIDTH;
    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv,
                                           &matrix_options, &runtime_opt))
    {
        return usage(argv[0]);
    }

    int num_carts = rand() % DEFAULT_MAX_NUM_CARTS + 1;

    int opt;
    bool rainbow = false;
    bool color_set = false;
    rgb_matrix::Color color{255, 255, 255};
    while ((opt = getopt(argc, argv, "c:C:r")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_carts = atoi(optarg);
            break;
        case 'C':
            color_set = true;
            if (!parseColor(&color, optarg))
            {
                fprintf(stderr, "Invalid color spec: %s\n", optarg);
                return usage(argv[0]);
            }
            break;
        case 'r':
            rainbow = true;
            break;
        default:
            return usage(argv[0]);
        }
    }

    if (rainbow && color_set)
    {
        fprintf(stderr, "Options -C & -r may not be used together!\n");
        return usage(argv[0]);
    }

    auto *rgb_mat = rgb_matrix::RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    if (rgb_mat == nullptr)
    {
        fprintf(stderr, "Failed to initialize rgb-matrix!\n");
        return -1;
    }

    auto *offscreen_canvas = rgb_mat->CreateFrameCanvas();
    rgb_mat->CreateFrameCanvas();

    int wheel_frame = 0;
    int smoke_frame = 0;

    auto *lok_static = calc_lok_matrix(num_carts);
    auto *smoke_0 = calc_smoke_matrix(0, num_carts);
    auto *smoke_1 = calc_smoke_matrix(1, num_carts);
    auto *wheels_0 = calc_wheel_matrix(0, num_carts);
    auto *wheels_1 = calc_wheel_matrix(1, num_carts);
    auto *wheels_2 = calc_wheel_matrix(2, num_carts);
    auto *wheels_3 = calc_wheel_matrix(3, num_carts);

    auto *smoke = &smoke_0;
    auto *wheels = &wheels_0;

    std::vector<int **> iw{
        wheels_0,
        wheels_1,
        wheels_2,
        wheels_3};

    int continuum = 0;
    int anim_len = ANIM_LENGTH - (DEFAULT_MAX_NUM_CARTS - num_carts) * ANIM_CART_OFFSET;
    for (int i = 0; i < anim_len; i++)
    {
        if (rainbow)
        {
            continuum += 5;
            continuum %= 3 * 255;
            int r = 0;
            int g = 0;
            int b = 0;
            if (continuum <= 255)
            {
                int c = continuum;
                b = 255 - c;
                r = c;
            }
            else if (continuum > 255 && continuum <= 511)
            {
                int c = continuum - 256;
                r = 255 - c;
                g = c;
            }
            else
            {
                int c = continuum - 512;
                g = 255 - c;
                b = c;
            }
            color.r = r;
            color.g = g;
            color.b = b;
        }

        // animate
        smoke_frame = i % 6 > 2 ? 1 : 0; // animate smoke every third frame
        wheel_frame = i % 4;

        smoke = smoke_frame ? &smoke_1 : &smoke_0;
        wheels = &iw[wheel_frame];

        // draw train
        offscreen_canvas = draw_lok(rgb_mat, offscreen_canvas, lok_static, *wheels, *smoke, color);

        // increment pointers
        for (size_t j = 0; j < ROWS; j++)
        {
            ++lok_static[j];
            ++wheels_0[j];
            ++wheels_1[j];
            ++wheels_2[j];
            ++wheels_3[j];
            ++smoke_0[j];
            ++smoke_1[j];
        }
    }

    delete rgb_mat;

    return 0;
}