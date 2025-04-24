#ifndef PRINTSETTINGSDMD2_H
#define PRINTSETTINGSDMD2_H

#include <QWidget>
#include <QSettings>
#include "PrintElements.h"

namespace Ui
{
  class printsettingsDMD2;
}

class printsettingsDMD2 : public QWidget
{
  Q_OBJECT

public:
  explicit printsettingsDMD2(QWidget *parent = nullptr);
  ~printsettingsDMD2();
  void initSettingsPointers(PrintSettings *pPrintSettings, PrintControls *pPrintControls,
                            PrintScripts *pPrintScript, InjectionSettings *pInjectionSettings);
  void savePrintSettings();
  void loadPrintSettings();
  void initPrintSettings();
  void logCurrentSettingsToFile();

  void printCurrentSettings();

  void EnableParameter(Parameter_t Parameter, bool State);
  int FileListCount();
  QString FileListItem(int itemNum);

Q_SIGNALS:
  void SettingsPrint(QString);
  void updateScript();

private slots:
void on_resinSelectDMD2_editTextChanged(const QString &arg1);
void on_StartingPositionParamDMD2_valueChanged(double arg1);
void on_BitDepthParamDMD2_valueChanged(int arg1);
void on_MaxImageUploadDMD2_valueChanged(int arg1);
void on_ResyncRateListDMD2_currentIndexChanged(const QString &arg1);
void on_LayerThicknessParamDMD2_valueChanged(double arg1);
void on_InitialExposureParameterDMD2_valueChanged(double arg1);
void on_InitialDelayParamDMD2_valueChanged(int arg1);
void on_UVIntensityParamDMD2_valueChanged(int arg1);
void on_InitialExposureIntensityParamDMD2_valueChanged(int arg1);
void on_ExposureTimeParamDMD2_valueChanged(double arg1);
void on_DarkTimeParamDMD2_valueChanged(double arg1);
void on_ContinuousMotionButtonDMD2_clicked();
void on_SteppedMotionButtonDMD2_clicked();
void on_pumpingCheckBoxDMD2_clicked();
void on_pumpingParameterDMD2_valueChanged(double arg1);
void on_StageVelocityParamDMD2_valueChanged(double arg1);
void on_MaxEndOfRunParamDMD2_valueChanged(double arg1);
void on_StageAccelParamDMD2_valueChanged(double arg1);
void on_MinEndOfRunParamDMD2_valueChanged(double arg1);
void on_ContinuousInjectionDMD2_clicked();
void on_SteppedContInjectionDMD2_clicked();
void on_BaseInfusionParamDMD2_valueChanged(double arg1);
void on_PreMovementCheckboxDMD2_clicked();
void on_PostMovementCheckboxDMD2_clicked();
void on_InjectionDelayParamDMD2_valueChanged(double arg1);
void on_InjectionRateParamDMD2_valueChanged(double arg1);
void on_VolPerLayerParamDMD2_valueChanged(double arg1);
void on_InitialVolumeParamDMD2_valueChanged(double arg1);
void on_UsePrintScriptDMD2_clicked();
void on_SelectPrintScriptDMD2_clicked();
void on_ClearPrintScriptDMD2_clicked();
void on_SelectFileDMD2_clicked();
void on_ClearImageFilesDMD2_clicked();
void on_PostExposureDelayParamDMD2_valueChanged(double arg1);
void on_JerkTimeParamDMD2_valueChanged(double arg1);

private:
  Ui::printsettingsDMD2 *ui;
  PrintSettings *psPrintSettingsDMD2;
  PrintControls *psPrintControlsDMD2;
  PrintScripts *psPrintScriptDMD2;
  InjectionSettings *psInjectionSettingsDMD2;

  void initImageFilesDMD2(QStringList file_names);
  void initPrintScript(QString file_name);
};

#endif // PRINTSETTINGSDMD2_H
