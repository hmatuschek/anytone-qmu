#include "device.hh"
#include "request.hh"
#include "response.hh"

#include <QIODevice>
#include "logger.hh"


Device::Device(QIODevice *interface, QObject *parent)
  : QObject{parent}, _state(State::Initial), _interface(interface), _in_buffer(), _out_buffer()
{
  if (nullptr == _interface)
    return;

  _interface->setParent(this);
  connect(_interface, SIGNAL(readyRead()), this, SLOT(onBytesAvailable()));
  connect(_interface, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));

  if (! _interface->open(QIODevice::ReadWrite)) {

  }
}

void
Device::onBytesAvailable() {
  _in_buffer.append(_interface->readAll());

  while (Request *req = Request::fromBuffer(_in_buffer)) {
    Response *resp = this->handle(req);
    delete req;
    if (resp) {
      if (resp->serialize(_out_buffer))
        onBytesWritten();
      delete resp;
    }
  }
}

void
Device::onBytesWritten() {
  if (0 == _out_buffer.size())
    return;

  qint64 nbytes = _interface->write(_out_buffer);
  _out_buffer.remove(0, nbytes);
}

Response *
Device::handle(Request *request) {
  if (request->is<ProgramRequest>()) {
    logDebug() << "Enter progam mode.";
    _state = State::Program;
    return new ProgramResponse();
  } else if ((State::Program == _state) && request->is<DeviceInfoRequest>()) {
    logDebug() << "Get device info.";
    return new DeviceInfoResponse(this->model(), this->hwVersion());
  } else if ((State::Program == _state) && request->is<ReadRequest>()) {
    ReadRequest *rreq = request->as<ReadRequest>();
    logDebug() << "Read " << (int)rreq->length() << "b from " << Qt::hex << rreq->address() << "h.";
    return new ReadResponse(rreq->address(), this->read(rreq->address(), rreq->length()));
  } else if ((State::Program == _state) && request->is<WriteRequest>()) {
    WriteRequest *wreq = request->as<WriteRequest>();
    logDebug() << "Write " << (int)wreq->payload().size() << "b to " << Qt::hex << wreq->address() << "h.";
    if (! this->write(wreq->address(), wreq->payload()))
      return nullptr;
    return new WriteResponse(wreq->address(), wreq->payload().size());
  } else if ((State::Program == _state) && request->is<EndRequest>()) {
    logDebug() << "Done.";
    _state = State::Initial;
    return nullptr;
  }

  logWarn() << "Uknown request.";
  return nullptr;
}

QByteArray
Device::read(uint32_t address, uint8_t len) {
  if (0x02fa0000 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x01\x01\x01\x00\x00\x01\x01\x20\x20\x20\x20\xff", 16);
  else if (0x02fa0010 == address)
    return QByteArray::fromRawData("\x44\x38\x37\x38\x55\x56\x00\x01\x00\xff\xff\xff\xff\xff\xff\xff", 16);
  else if (0x02fa0020 == address)
    return QByteArray::fromRawData("\xff\xff\xff\xff\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa0030 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa0040 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa0050 == address)
    return QByteArray::fromRawData("\x31\x32\x33\x34\x35\x36\x37\x38\xff\xff\xff\xff\xff\xff\xff\xff", 16);
  else if (0x02fa0060 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa0070 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa0080 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa0090 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa00a0 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa00b0 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa00c0 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa00d0 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa00e0 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
  else if (0x02fa00f0 == address)
    return QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);

  logWarn() << "Cannot resolve address " << Qt::hex << address << "h. Respond with 0h.";
  return QByteArray(16,0);
}

bool
Device::write(uint32_t addr, const QByteArray &data) {
  return true;
}
