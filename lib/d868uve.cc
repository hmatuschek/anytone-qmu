#include "d868uve.hh"

D868UVE::D868UVE(QIODevice *interface, QObject *parent)
  : Device{interface, parent}
{
  // pass...
}

QByteArray
D868UVE::model() const {
  return QByteArray::fromRawData("868UVE", 6);
}

QByteArray
D868UVE::hwVersion() const {
  return QByteArray::fromRawData("V102\x00\x00", 6);
}
