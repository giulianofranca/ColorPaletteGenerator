#include "headers/generator.hpp"

#include "ui/ui_generator.h"
#include "headers/moc_generator.cpp"

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#error Some of QImage's formats are endian-dependant. This file assumes little endian. Source: https://github.com/hey-frnk/ColorPaletteGenerator/blob/master/asmOpenCV.h
#endif

Generator::Generator(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::Generator){
    ui->setupUi(this);
    this->setWindowTitle("Color Palette Generator");

    kUserIsResizing = false;

    installEventFilter(this);
    connect(ui->btnLoad, SIGNAL(clicked()), this, SLOT(loadImageClicked()));
    generatePaletteClicked();
}

Generator::~Generator(){
    removeEventFilter(this);
    delete ui;
}

void Generator::generatePaletteClicked(){
    // Parse image
    cv::Mat sourceImg = ASM::QImageToCvMat(curImg.rgbSwapped());
    // Serialize, float
    // Perform K-Means
}

void Generator::loadImageClicked(){
    QFileInfo thisFile = QFileInfo(QString(__FILE__));
    QDir imgDir(thisFile.absolutePath());
    imgDir.cdUp();
    imgDir.cd("img");
    QString imgPath = QFileDialog::getOpenFileName(this, tr("Open Image"), imgDir.absolutePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    QImage img(imgPath);
    curImg = img.scaled(ui->lblImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap pix = QPixmap::fromImage(curImg);
    ui->lblImage->setPixmap(pix.scaled(ui->lblImage->size(), 
        Qt::KeepAspectRatio, Qt::SmoothTransformation));
    qDebug() << imgPath;
}

void Generator::resizeImage(){
    if (!curImg.isNull()){
        QPixmap pix = QPixmap::fromImage(curImg);
        ui->lblImage->setPixmap(pix.scaled(ui->lblImage->size(),
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

bool Generator::eventFilter(QObject *obj, QEvent *event){
    if ((event->type() == QEvent::MouseButtonRelease) || 
        (event->type() == QEvent::NonClientAreaMouseButtonRelease)){
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if ((mouseEvent->button() == Qt::LeftButton) && kUserIsResizing){
            kUserIsResizing = false;
            resizeImage();
        }
    }
    return QObject::eventFilter(obj, event);
}

void Generator::changeEvent(QEvent *event){
    if (event->type() == QEvent::WindowStateChange){
        if (event->type() == QEvent::WindowStateChange){
            QWindowStateChangeEvent *windowEvent = dynamic_cast<QWindowStateChangeEvent*>(event);
            if (windowEvent->oldState() & Qt::WindowMaximized){
                resizeImage();
            }
            else if (isMaximized()){
                resizeImage();
            }
        }
    }
}

void Generator::resizeEvent(QResizeEvent *event){
    kUserIsResizing = true;
}

// cv::Mat Generator::QImageToCvMat(const QImage &inImage, bool cloneImgData=true){
//     switch (inImage.format()){

//         // 32-bit, 4 channel
//         case QImage::Format_ARGB32:
//         case QImage::Format_ARGB32_Premultiplied:{
//             cv::Mat mat(
//                 inImage.height(), inImage.width(),
//                 CV_8UC4,
//                 const_cast<uchar*>(inImage.bits()),
//                 static_cast<size_t>(inImage.bytesPerLine())
//             );
//             return (cloneImgData ? mat.clone() : mat);
//         }
        
//         // 32-bit, 3 channel
//         case QImage::Format_RGB32:{
//             if (!cloneImgData)
//                 qWarning() << "QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
//             cv::Mat mat(
//                 inImage.height(), inImage.width(),
//                 CV_8UC4,
//                 const_cast<uchar*>(inImage.bits()),
//                 static_cast<size_t>(inImage.bytesPerLine())
//             );
//             cv::Mat matNoAlpha;
//             cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR); // drop the all-white alpha channel
//             return matNoAlpha;
//         }

//         // 8-bit, 3 channel
//         case QImage::Format_RGB888:{
//             if (!cloneImgData)
//                 qWarning() << "QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
//             QImage swapped = inImage.rgbSwapped();
//             cv::Mat mat(
//                 swapped.height(), swapped.width(),
//                 CV_8UC3,
//                 const_cast<uchar*>(swapped.bits()),
//                 static_cast<size_t>(swapped.bytesPerLine())
//             );
//             return mat.clone();
//         }

//         // 8-bit, 1 channel
//         case QImage::Format_Indexed8:{
//             cv::Mat mat(
//                 inImage.height(), inImage.width(),
//                 CV_8UC1,
//                 const_cast<uchar*>(inImage.bits()),
//                 static_cast<size_t>(inImage.bytesPerLine())
//             );
//             return (cloneImgData ? mat.clone() : mat);
//         }

//         default:
//             qWarning() << "QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
//             break;
//     }
//     return cv::Mat();
// }