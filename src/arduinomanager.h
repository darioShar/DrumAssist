#ifndef ARDUINOMANAGER_H
#define ARDUINOMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#define ARDUINO_DEBUG

class ArduinoManager : public QObject
{
    Q_OBJECT
public:
    ArduinoManager();
    ~ArduinoManager();

    bool initialize();
    bool scanForArduino();
    bool configureSerialPort();
    bool checkSerialPortError();
    bool isAvailable() { return m_arduino_is_available; }

public slots :
    bool writeByteToArduino(char);

signals :
    void arduinoPortName(QString) const;

private :
    static const quint16    m_arduino_uno_vendor_id = 9025;
    static const quint16    m_arduino_uno_product_id = 66;
    QSerialPort*            m_arduino;
    QString                 m_arduino_port_name;
    bool                    m_arduino_is_available;
};

#endif // ARDUINOMANAGER_H


