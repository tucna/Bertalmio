#include <QCoreApplication>
#include <QImage>

#include "bertalmioprocessing.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BertalmioProcessing bertalmioParts;

    QImage input;
    QImage anisotropic;
    QImage laplace;
    QImage gradient;

    anisotropic = bertalmioParts.anisotropicDiffusion(input);
    laplace = bertalmioParts.laplace(input);
    gradient = bertalmioParts.gradient(input);

    return a.exec();
}
