// https://towardsdatascience.com/introduction-to-image-segmentation-with-k-means-clustering-83fd0a9e2fc3

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <QtCore/QEvent>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <QtCore/QDebug>
#include <QtCore/QtGlobal>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Generator; }
QT_END_NAMESPACE

class Generator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Generator(QWidget *parent = nullptr);
    ~Generator();

protected:
    bool                                eventFilter(QObject *obj, QEvent *event) override;
    void                                resizeEvent(QResizeEvent *event) override;
    void                                changeEvent(QEvent *event) override;


private slots:
    void                                generatePaletteClicked();
    void                                loadImageClicked();

private:
    void                                resizeImage();
    // cv::Mat                             QImageToCvMat(const QImage &inImage, 
    //                                                   bool cloneImgData);

    Ui::Generator                       *ui;
    QString                             imgPath;
    QImage                              curImg;
    bool                                kUserIsResizing;
};


namespace ASM{
    inline cv::Mat QImageToCvMat(const QImage &inImage, bool cloneImgData=true){
        switch (inImage.format()){

            // 32-bit, 4 channel
            case QImage::Format_ARGB32:
            case QImage::Format_ARGB32_Premultiplied:{
                cv::Mat mat(
                    inImage.height(), inImage.width(),
                    CV_8UC4,
                    const_cast<uchar*>(inImage.bits()),
                    static_cast<size_t>(inImage.bytesPerLine())
                );
                return (cloneImgData ? mat.clone() : mat);
            }
            
            // 32-bit, 3 channel
            case QImage::Format_RGB32:{
                if (!cloneImgData)
                    qWarning() << "QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
                cv::Mat mat(
                    inImage.height(), inImage.width(),
                    CV_8UC4,
                    const_cast<uchar*>(inImage.bits()),
                    static_cast<size_t>(inImage.bytesPerLine())
                );
                cv::Mat matNoAlpha;
                cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR); // drop the all-white alpha channel
                return matNoAlpha;
            }

            // 8-bit, 3 channel
            case QImage::Format_RGB888:{
                if (!cloneImgData)
                    qWarning() << "QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
                QImage swapped = inImage.rgbSwapped();
                cv::Mat mat(
                    swapped.height(), swapped.width(),
                    CV_8UC3,
                    const_cast<uchar*>(swapped.bits()),
                    static_cast<size_t>(swapped.bytesPerLine())
                );
                return mat.clone();
            }

            // 8-bit, 1 channel
            case QImage::Format_Indexed8:{
                cv::Mat mat(
                    inImage.height(), inImage.width(),
                    CV_8UC1,
                    const_cast<uchar*>(inImage.bits()),
                    static_cast<size_t>(inImage.bytesPerLine())
                );
                return (cloneImgData ? mat.clone() : mat);
            }

            default:
                qWarning() << "QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
                break;
        }
        return cv::Mat();
    }
};


#endif // GENERATOR_HPP