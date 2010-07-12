
// QT includes
#include <QSortFilterProxyModel>

// CTK includes
#include "ctkModelTester.h"

// qMRML includes
#include "qMRMLListWidget.h"
//#include "qMRMLItemModel.h"
#include "qMRMLSceneModel.h"
#include "qMRMLSceneTransformModel.h"

//------------------------------------------------------------------------------
class qMRMLListWidgetPrivate: public ctkPrivate<qMRMLListWidget>
{
public:
  CTK_DECLARE_PUBLIC(qMRMLListWidget);
  void init();
};

//------------------------------------------------------------------------------
void qMRMLListWidgetPrivate::init()
{
  CTK_P(qMRMLListWidget);
  //p->QListView::setModel(new qMRMLItemModel(p));
  //p->QListView::setModel(new qMRMLSceneModel(p));
  ///new ctkModelTester(p->model(), p);
  
  qMRMLSceneTransformModel* sceneModel = new qMRMLSceneTransformModel(p);
  QSortFilterProxyModel* sortModel = new QSortFilterProxyModel(p);
  sortModel->setSourceModel(sceneModel);
  sortModel->setDynamicSortFilter(true);
  p->QListView::setModel(sortModel);
  p->setWrapping(true);
  p->setResizeMode(QListView::Adjust);
  p->setFlow(QListView::TopToBottom);
  // We have a problem when the model is reset (qMRMLSceneModel::setMRMLScene(0)), 
  // the QSortFilterProxyModel doesn't realize that the rows have disappeared 
  // and QSortFilterProxyModel::rowCount(QModelIndex) returns 1(the mrmlscene), which
  // is eventually called by the ctkModelTester slot connected to QSortFilterProxyModel
  // signal layoutAboutToBeChanged() which eventually calls testData on the valid QModelIndex
  //new ctkModelTester(p->model(), p);
    
  //ctkModelTester* tester = new ctkModelTester(p);
  //tester->setModel(transformModel);
}

//------------------------------------------------------------------------------
qMRMLListWidget::qMRMLListWidget(QWidget *_parent)
  :QListView(_parent)
{
  CTK_INIT_PRIVATE(qMRMLListWidget);
  ctk_d()->init();
}

//------------------------------------------------------------------------------
qMRMLListWidget::~qMRMLListWidget()
{

}

//------------------------------------------------------------------------------
void qMRMLListWidget::setMRMLScene(vtkMRMLScene* scene)
{
  QSortFilterProxyModel* sortModel = qobject_cast<QSortFilterProxyModel*>(this->model());
  qMRMLSceneModel* mrmlModel = qobject_cast<qMRMLSceneModel*>(sortModel->sourceModel());
  Q_ASSERT(mrmlModel);

  mrmlModel->setMRMLScene(scene);
  if (scene)
    {
    this->setRootIndex(sortModel->index(0, 0));
    sortModel->sort(0);
    sortModel->invalidate();
    }
}

//------------------------------------------------------------------------------
vtkMRMLScene* qMRMLListWidget::mrmlScene()const
{
  QSortFilterProxyModel* sortModel = qobject_cast<QSortFilterProxyModel*>(this->model());
  Q_ASSERT(qobject_cast<const qMRMLSceneModel*>(sortModel->sourceModel()));
  return qobject_cast<const qMRMLSceneModel*>(sortModel->sourceModel())->mrmlScene();
}
