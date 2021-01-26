#include "arduinomanager.h"

ArduinoManager::ArduinoManager()
    :   m_arduino_is_available(false)
    ,   m_arduino_port_name("")
    ,   m_arduino(new QSerialPort)
{
#ifdef ARDUINO_DEBUG
    qWarning() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
#endif
}

ArduinoManager::~ArduinoManager() {
    m_arduino->close();
    delete m_arduino;
}

bool ArduinoManager::initialize(){
    if (scanForArduino())
        if(configureSerialPort())
            return true;

    return false;
}

bool ArduinoManager::scanForArduino() {
    m_arduino_is_available = false;
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
#ifdef ARDUINO_DEBUG
            qWarning() << "Serial port found. Specs :"
                       << "Vendor ID : " << serialPortInfo.vendorIdentifier()
                       << ".Product ID :" << serialPortInfo.productIdentifier();
#endif
            if(serialPortInfo.vendorIdentifier() == m_arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == m_arduino_uno_product_id){
                    m_arduino_port_name = serialPortInfo.portName();
                    emit arduinoPortName(m_arduino_port_name);
                    m_arduino_is_available = true;
                    return true;
                }
            }
        }
    }
    emit arduinoPortName("<font color='red'>No Arduino Available !</font color>");
    return false;
}

bool ArduinoManager::configureSerialPort() {
    if(m_arduino_is_available){
        // open and configure the serialport
        m_arduino->setPortName(m_arduino_port_name);
        m_arduino->open(QSerialPort::ReadWrite);
        m_arduino->setBaudRate(QSerialPort::Baud9600);
        m_arduino->setDataBits(QSerialPort::Data8);
        m_arduino->setParity(QSerialPort::NoParity);
        m_arduino->setStopBits(QSerialPort::OneStop);
        m_arduino->setFlowControl(QSerialPort::NoFlowControl);
        return true;
    }
    return false;
}

bool ArduinoManager::checkSerialPortError(){
    if (m_arduino->error()) {
#ifdef ARDUINO_DEBUG
        qWarning() << "An error ocurred during serial port operation. Error code :" << m_arduino->error();
#endif
        m_arduino->close();
        emit arduinoPortName("<font color='red'>No Arduino connected !</font color>");
        return true;
    }
    return false;
}

bool ArduinoManager::writeByteToArduino(char value) {
    // Check if there is an error code (port disconnected for instance...)
    if(checkSerialPortError()){
        qWarning() << "Serial Port Error : Arduino not connected or Special Error";
        return false;
    }

    if (m_arduino->isOpen() && m_arduino->isWritable()) {
#ifdef ARDUINO_DEBUG
        qWarning() << "Arduino is open and writable.";
        qWarning() << "Writing" << QString::number(value) << "to the Arduino";
#endif
        m_arduino->write(&value, 1);
        if (m_arduino->bytesToWrite() > 0) {
           if(m_arduino->flush()) {
#ifdef ARDUINO_DEBUG
                qWarning() << "Data has been written to Arduino";
#endif
            }
        }
        return true;
    }
    return false;
}
