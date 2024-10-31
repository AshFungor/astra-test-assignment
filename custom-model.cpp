// QT-es
#include <QDirIterator>

// STD
#include <memory>

#include "custom-model.h"

namespace {

    std::size_t countBytes(const QString& dir) {
        std::size_t size = 0;
        QDirIterator iter {dir, QDirIterator::Subdirectories};
        while (iter.hasNext()) {
            QFileInfo info = iter.fileInfo();
            iter.next();
            if (info.isSymLink()) {
                continue;
            }
            size += info.size();
        }
        return size;
    }

}

CustomModel::CustomModel(QObject *parent)
    : QFileSystemModel{parent}
    , button_(std::make_unique<QPushButton>())
{}

QVariant CustomModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QFileSystemModel::data(index, role);
    }
    if (index.column() == columnCount() - 1) {
       switch(role) {
          case(Qt::DisplayRole):
           return QString{"lol"};
          case(Qt::TextAlignmentRole):
              return Qt::AlignHCenter;
          default:
              return {};
       }
    }
    return QFileSystemModel::data(index,  role);
}

QVariant CustomModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (section == columnCount() - 1) {
       switch(role) {
          case(Qt::DisplayRole):
              return QString{"Размер папки"};
          case(Qt::TextAlignmentRole):
              return Qt::AlignHCenter;
          default:
              return {};
       }
    }
    return QFileSystemModel::headerData(section, orientation, role);
}

int CustomModel::columnCount(const QModelIndex& parent) const {
    return QFileSystemModel::columnCount(parent) + 1;
}
