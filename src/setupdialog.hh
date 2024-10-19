#ifndef SETUPDIALOG_HH
#define SETUPDIALOG_HH

#include <QDialog>
#include "modeldefinition.hh"

class Device;

namespace Ui {
  class SetupDialog;
}

class SetupDialog : public QDialog
{
  Q_OBJECT

public:
  enum class Interface {
    PTY, Serial
  };
  Q_ENUM(Interface)

public:
  explicit SetupDialog(QWidget *parent = nullptr);
  ~SetupDialog();

  Device *createDevice(const ErrorStack &err=ErrorStack());

private slots:
  void onDeviceSelected(int idx);
  void onInterfaceSelected(int idx);
  void onUseBuildinPatternToggled(bool enabled);
  void onSelectCatalogFile();

private:
  Ui::SetupDialog *ui;
  ModelCatalog _catalog;
};

#endif // SETUPDIALOG_HH
