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
    const int T = 50;  // repetition of loops A, B (A*B*T = 15*2*50 = 1500 ===> 2.6 secs) - 50
    const float dt = 0.5; // 0.5

    /*
    float I[N][M] = {
        {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3, 4, 5, 6, 7,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3, 4, 5, 6, 7,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3, 4, 5, 6,10,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3, 4, 5, 6,10,10, 9, 8, 7, 6, 5, 4,10, 2, 1, 0, 0},
        {0, 0, 1, 2, 3, 4, 5,10,10, 8, 9, 8, 7, 6, 5,10,10, 2, 1, 0, 0},
        {0, 0, 1, 2, 3,10,10, 6,10,10,10,10,10,10,10,10,10,10, 1, 0, 0},
        {0, 0, 1, 2, 3,10,10, 6, 7, 8, 9, 8,10,10,10,10,10,10, 1, 0, 0},
        {0, 0, 1, 2,10,10, 5, 6, 7, 8, 9, 8, 7, 6,10,10,10, 2, 1, 0, 0},
        {0, 0,10,10, 3,10, 5, 6, 7, 8, 9, 8, 7, 6, 5,10, 3, 2, 1, 0, 0},
        {0, 0,10,10, 3,10, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4,10, 2, 1, 0, 0},
        {0, 0, 1, 2, 3,10, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4,10, 2, 1, 0, 0},
        {0, 0, 1, 2, 3,10, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3,10, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3,10, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0},
        {0, 0, 1, 2, 3,10, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0}
    };

    float mask_bert[N][M] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

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
    BertalmioProcessing::List2DFloat Mask_bert = bertalmioParts.maskToFloat(M_bert);
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

        QImage result = bertalmioParts.floatToImage(Inpainting_bert);
        result.save("output"+ QString::number(t) +".png");


        /*
        // Diffusion
        for (int b = 0; b < B; b++)
        {
            bertalmioParts.anisotropicDiffusion_3(Inpainting_bert);
        }
        */
    }

    /*
    for (int y = 0; y < N; y++)
    {
        qDebug() << Inpainting_bert.r[y];
    }
    */

    return a.exec();
}
