#ifndef BLENDERTAB_H
#define BLENDERTAB_H

#include <QWidget>
#include <ui_BlenderTab.h>

#include "AbstractDocumentTab.h"

class qavimator;
class QCloseEvent;
class QStringList ;


/*namespace Ui {
    class BlenderTab;
}*/

class BlenderTab : public QWidget, public Ui::BlenderTab, public AbstractDocumentTab {
  Q_OBJECT

  public:
    BlenderTab(/*QWidget *parent = 0*/qavimator* mainWindow, const QString& fileName, bool createFile);
    ~BlenderTab();

    virtual void AddFile();
    virtual void Save();
    virtual void SaveAs();
    virtual void Cut();
    virtual void Copy();
    virtual void Paste();
    virtual void Undo() {/*TODO*/};
    virtual void Redo() {/*TODO*/};
    virtual void ResetView();
    virtual void ExportForSecondLife();
    virtual void UpdateToolbar();
    virtual void UpdateMenu();
    virtual bool IsUnsaved();

  signals:
     void resetCamera();

  public slots:
     virtual void onTabActivated();
     void on_animsList_AnimationFileTaken(QString filename);

  protected:
     // prevent closing of main window if there are unsaved changes
     virtual void closeEvent(QCloseEvent* event);

     void setCurrentFile(const QString& fileName);
     QString selectFileToOpen(const QString& caption);
     void fileNew();
     void fileOpen();
     void fileOpen(const QString& fileName);
     void fileSaveAs();

     //edu: add AVM (also BVH?)
     void fileAdd();
     void fileAdd(const QString& fileName);

     void fileExportForSecondLife();
     bool resolveUnsavedChanges();

     void editCut();
     void editCopy();
     void editPaste();

  private:
     void bindMenuActions();

     /////////////edu: DEBUG /////////////
     void sorry();
};

#endif // BLENDERTAB_H