#include <QCoreApplication>
#include <QImage>
#include <QDebug>

#include "bertalmioprocessing.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BertalmioProcessing bertalmioParts;

    QImage input(QCoreApplication::applicationDirPath() + "/lena.png");

    if (input.isNull())
    {
        qDebug() << "Image not found!";
    }
    else
    {
        QImage anisotropic;
        QImage laplace;
        QImage gradientInput;

        BertalmioProcessing::GradientLaplace gradientLaplace;

        laplace = bertalmioParts.laplace(input);
        gradientLaplace = bertalmioParts.gradientLaplace(laplace);

        // GradientLaplace starts on 1,1 !!

        qDebug() << gradientLaplace.r[1][1].x << gradientLaplace.r[1][1].y;

        anisotropic = bertalmioParts.anisotropicDiffusion(input);
        gradientInput = bertalmioParts.gradientInput(input);

        laplace.save("laplace.png");
    }

    return a.exec();
}
