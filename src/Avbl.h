#ifndef AVBL_H
#define AVBL_H

#include <QList>
#include <QString>

class QDomDocument;
class QDomElement;
class BVHNode;
class TimelineTrail;


class Avbl
{
public:
  Avbl();

  /** Returns TRUE on success */
  bool SaveToFile(QList<TimelineTrail*> trails, QString fileName);

  bool HasErrors() const { return hasErrors; }
  QString ErrorMessage() const { return errorMessage; }


private:
  bool hasErrors;
  QString errorMessage;

  void createBoneWeightsElement(QDomDocument document, QDomElement parentElement,
                                BVHNode* limb, int frames);
};

#endif // AVBL_H
