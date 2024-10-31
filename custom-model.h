#pragma once

#include <QFileSystemModel>
#include <QObject>
#include <QWidget>
#include <QPushButton>

#include <map>
#include <memory>

class CustomModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit CustomModel(QObject *parent = nullptr);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::map<QString, std::size_t> sizes_;
    std::unique_ptr<QPushButton> button_;

};
