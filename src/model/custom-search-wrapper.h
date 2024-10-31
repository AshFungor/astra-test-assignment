#pragma once

// STD
#include <memory>

// Qt-es
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

namespace test {

    class CustomSearchWrapper
        : public QSortFilterProxyModel {
        Q_OBJECT
    public:

        // this wrapper has non-owning reference to search test, which is updated on signal
        explicit CustomSearchWrapper(std::weak_ptr<QString> search, QObject *parent = nullptr);

        // QSortFilterProxyModel implementation
        virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

    private:
        std::size_t metric(std::shared_ptr<QString> target, const QString& source) const;

    private:
        std::weak_ptr<QString> search_;

    };

}