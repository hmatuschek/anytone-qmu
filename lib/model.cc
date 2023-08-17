#include "model.hh"
#include "image.hh"
#include "logger.hh"


/* ********************************************************************************************* *
 * Implementation of Model
 * ********************************************************************************************* */
Model::Model(QObject *parent)
  : QObject{parent}
{
  // pass...
}


bool
Model::read(uint32_t address, uint8_t length, QByteArray &payload) {
  Q_UNUSED(address); Q_UNUSED(length); Q_UNUSED(payload)
  return false;
}

bool
Model::write(uint32_t address, const QByteArray &payload) {
  Q_UNUSED(address); Q_UNUSED(payload)
  return false;
}

void
Model::startProgram() {
  // pass...
}

void
Model::endProgram() {
  // pass...
}


/* ********************************************************************************************* *
 * Implementation of ImageCollector
 * ********************************************************************************************* */
ImageCollector::ImageCollector(QObject *parent)
  : Model(parent), _images()
{
  // pass...
}

unsigned int
ImageCollector::count() const {
  return _images.count();
}

const Image *
ImageCollector::image(unsigned int idx) const {
  return _images.at(idx);
}

bool
ImageCollector::write(uint32_t address, const QByteArray &payload) {
  if (0 == count()) {
    logError() << "No image created yet.";
    return false;
  }
  _images.last()->append(address, payload);
  return true;
}

void
ImageCollector::startProgram() {
  if ((0 == _images.count()) || (0 != _images.last()->count())) {
    logInfo() << "Create new image.";
    _images.append(new Image(this));
  } else if (0 == _images.last()->count()) {
    logInfo() << "Reuse last image.";
  }
}

void
ImageCollector::endProgram() {
  if ((0 != _images.count()) && (0 != _images.last()->count())) {
    logInfo() << "Image received.";
    emit imageReceived();
  }
}

