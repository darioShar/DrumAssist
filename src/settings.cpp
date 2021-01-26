#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QAudioFormat * const format, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    m_format(format)
{
    ui->setupUi(this);

    ui->comboBoxAudioCodec->addItem("PCM", QVariant(tr("audio/pcm")));

    ui->comboBoxSampleRate->addItem("8000 Hz", QVariant(8000));
    ui->comboBoxSampleRate->addItem("9600 Hz", QVariant(9600));
    ui->comboBoxSampleRate->addItem("19200 Hz", QVariant(19200));
    ui->comboBoxSampleRate->addItem("38400 Hz", QVariant(38400));
    ui->comboBoxSampleRate->addItem("44100 Hz", QVariant(44100));
    ui->comboBoxSampleRate->addItem("48000 Hz", QVariant(48000));
    ui->comboBoxSampleRate->addItem("96000 Hz", QVariant(96000));
    ui->comboBoxSampleRate->addItem("192000 Hz", QVariant(192000));
    ui->comboBoxSampleRate->setCurrentIndex(5);

    ui->comboBoxSampleSize->addItem("8 bits", QVariant(8));
    ui->comboBoxSampleSize->addItem("16 bits", QVariant(16));
    ui->comboBoxSampleSize->addItem("32 bits", QVariant(32));
    ui->comboBoxSampleSize->setCurrentIndex(1);

    ui->comboBoxSampleType->addItem("Int", QVariant(QVariant::Int));
    ui->comboBoxSampleType->addItem("Unsigned Int", QVariant(QVariant::UInt));
    ui->comboBoxSampleType->addItem("Float", QVariant(QVariant::Double)); // not a double but no choice
    ui->comboBoxSampleType->setCurrentIndex(0);

}

Settings::~Settings()
{
    close();
    delete ui;
}


void Settings::on_comboBoxAudioMode_currentIndexChanged(int i) {
    ui->sliderFrequency->setEnabled(i == 2 ? true : false);
    if (i == 1) m_format->setSampleRate(1058000); // Ouai c'est vraiment beaucoup ouai.
    else m_format->setSampleRate(ui->comboBoxSampleRate->currentData().toInt());
    emit audioModeChanged(i);
}

void Settings::on_sliderFrequency_valueChanged(int i) {
    QString freq = QString::number(i);
    int space = std::max(4 - freq.size(), 0);
    for (int k = 0; k < space; k++) freq += " ";
    ui->labelFrequency->setText("Generated Frequency (" + freq + " Hz)");

    emit generatedFrequencyChanged(i);
}

void Settings::on_comboBoxAudioCodec_currentIndexChanged(int i) {
    Q_UNUSED(i);
    m_format->setCodec(ui->comboBoxAudioCodec->currentData().toString());
    emit audioCodecChanged() ;
}

void Settings::on_comboBoxSampleRate_currentIndexChanged(int i) {
    Q_UNUSED(i);
    m_format->setSampleRate(ui->comboBoxSampleRate->currentData().toInt());
    emit sampleRateChanged();
}

void Settings::on_comboBoxSampleSize_currentIndexChanged(int i) {
    Q_UNUSED(i);
    m_format->setSampleSize(ui->comboBoxSampleSize->currentData().toInt());
    emit sampleSizeChanged() ;
}

void Settings::on_comboBoxSampleType_currentIndexChanged(int i) {
    Q_UNUSED(i);
    // For now we won't do that (problem with audioInfo reinit...)
    //m_format->setSampleType(ui->comboBoxSampleType->currentData());
    emit sampleTypeChanged() ;
}

void Settings::on_comboBoxWindowFunction_currentIndexChanged(int i) {
    emit windowFunctionChanged(i) ;
}
