#include <QCoreApplication>
#include <QImage>
#include <QDebug>

#include "bertalmioprocessing.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BertalmioProcessing bertalmioParts;

    const int M = 21;
    const int N = 17;
    const int A = 15;  // steps of inpainting with equation (4) - 15
    const int B = 2;   // steps of diffusion with equation (3) - 2
    const int T = 1000;  // repetition of loops A, B (A*B*T = 15*2*50 = 1500 ===> 2.6 secs) - 50
    const float dt = 0.5; // 0.5

    /*
    float I[N][M] = {
        {0, 0,26,51,77,102,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,255,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,255,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,255,255,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,255,255,255,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,255,255,255,255,255,255,255,255,255,255,255,26, 0, 0},
        {0, 0,26,51,77,255,255,153,179,204,230,204,179,153,128,102,77,255,26, 0, 0},
        {0, 0,26,51,255,255,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,255,255,77,255,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,255,255,77,255,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,255,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0},
        {0, 0,26,51,77,102,128,153,179,204,230,204,179,153,128,102,77,51,26, 0, 0}
    };

    float mask_bert[N][M] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0,255,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0,255,255, 0, 0, 0, 0, 0, 0,255, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0,255,255, 0, 0, 0, 0, 0, 0,255,255, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,255,255, 0,255,255,255,255,255,255,255,255,255,255, 0, 0, 0},
        {0, 0, 0, 0, 0,255,255, 0, 0, 0, 0, 0,255,255,255,255,255,255, 0, 0, 0},
        {0, 0, 0, 0,255,255, 0, 0, 0, 0, 0, 0, 0, 0,255,255,255, 0, 0, 0, 0},
        {0, 0,255,255, 0,255, 0, 0, 0, 0, 0, 0, 0, 0, 0,255, 0, 0, 0, 0, 0},
        {0, 0,255,255, 0,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,255, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,255, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    */

    /*
    // Normalization
    for (int y = 0; y < N; y++)
        for (int x = 0; x < M; x++)
        {
            I[y][x] = I[y][x] / (float)10;
        }
    */

    QImage I_bert("I_bert.png");
    QImage M_bert("mask_bert.png");

    // Recomputation
    BertalmioProcessing::List2DFloat Inpainting_bert = bertalmioParts.imageToFloat(I_bert);
    BertalmioProcessing::List2DFloat Mask_bert = bertalmioParts.imageToFloat(M_bert);

    //BertalmioProcessing::List2DFloat Inpainting_bert = bertalmioParts.array2DToFloat(I, N);
    //BertalmioProcessing::List2DFloat Mask_bert = bertalmioParts.array2DToFloat(mask_bert, N);

    BertalmioProcessing::List2DFloat L_bert;
    BertalmioProcessing::List2DFloat mod_grad_mag_bert;
    BertalmioProcessing::List2DFloat It_bert;
    BertalmioProcessing::List2DFloat beta_bert;
    BertalmioProcessing::Gradient IxIy_bert;
    BertalmioProcessing::Gradient LxLy_bert;

    for (int t = 0; t < T; t++)
    {
        qDebug() << "New iteration..." << t;

        // Inpainting
        for (int aa = 0; aa < A; aa++)
        {
            IxIy_bert = bertalmioParts.gradient(Inpainting_bert);
            L_bert = bertalmioParts.laplace_7(Inpainting_bert);
            LxLy_bert = bertalmioParts.gradient(L_bert);

            beta_bert = bertalmioParts.beta_9(LxLy_bert, IxIy_bert);
            mod_grad_mag_bert = bertalmioParts.gradientInput_10(Inpainting_bert, beta_bert, Mask_bert);
            It_bert = bertalmioParts.partialResult_5(beta_bert, mod_grad_mag_bert);

            /*
            QString debugS = "";

            for (int y = 0; y < N; y++)
            {
                for (int x = 0; x < M; x++)
                {
                    debugS += QString::number(beta_bert.r[y][x]) + "\t";
                }

                qDebug() << debugS;
                debugS = "";
            }
            */

            bertalmioParts.updateImage_4(Inpainting_bert, It_bert, dt);
        }

        /*
        // Diffusion
        for (int b = 0; b < B; b++)
        {
            bertalmioParts.anisotropicDiffusion_3(Inpainting_bert);
        }
        */

        QImage result = bertalmioParts.floatToImage(Inpainting_bert);
        result.save("output"+ QString::number(t) +".png");
    }

    /*
    for (int y = 0; y < N; y++)
    {
        qDebug() << Inpainting_bert.r[y];
    }
    */

    return a.exec();
}
