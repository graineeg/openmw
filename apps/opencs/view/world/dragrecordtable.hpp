#ifndef CSV_WORLD_DRAGRECORDTABLE_H
#define CSV_WORLD_DRAGRECORDTABLE_H

#include <QTableView>

class QWidget;
class QAction;

namespace CSMDoc
{
    class Document;
}

namespace CSMWorld
{
    class UniversalId;
}

namespace CSVWorld
{
    class DragRecordTable : public QTableView
    {
        protected:
            CSMDoc::Document& mDocument;

        public:
            DragRecordTable(CSMDoc::Document& document, QWidget* parent = NULL);

            virtual std::vector<CSMWorld::UniversalId> getDragedRecords() const = 0;

        protected:
            void startDrag(const DragRecordTable& table);
    };
}

#endif

