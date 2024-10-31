// QT-es
#include <QVariant>

// STD
#include <map>
#include <algorithm>

#include <src/model/custom-search-wrapper.h>

namespace {

    std::map<QChar, int>& count(std::map<QChar, int>& map, const QString& source) {
        for (auto& ch : source) {
            if (map.find(ch) != map.end()) {
                ++map[ch];
            } else {
                map[ch] = 1;
            }
        }
        return map;
    }

    int compare(std::map<QChar, int>& target, std::map<QChar, int>& source) {
        int sum = 0;
        for (auto& kv_pair : target) {
            if (source.find(kv_pair.first) == source.end()) {
                continue;
            }

            sum += std::min(source[kv_pair.first], kv_pair.second);
        }
        return sum;
    }

}

test::CustomSearchWrapper::CustomSearchWrapper(std::weak_ptr<QString> search, QObject *parent)
    : QSortFilterProxyModel{parent}
    , search_(std::move(search))
{}

bool test::CustomSearchWrapper::lessThan(const QModelIndex& left, const QModelIndex& right) const {
    QFileSystemModel *model = qobject_cast<QFileSystemModel*>(sourceModel());

    QFileInfo leftFile = model->fileInfo(left);
    QFileInfo rightFile = model->fileInfo(right);

    if (auto locked = search_.lock()) {
        std::size_t first = metric(locked, leftFile.fileName().toLower());
        std::size_t second = metric(locked, rightFile.fileName().toLower());

        if (first == second) {
            return leftFile.fileName() < rightFile.fileName();
        }
        return first < second;
    }

    // does not matter at this point
    return true;
}

std::size_t test::CustomSearchWrapper::metric(std::shared_ptr<QString> target, const QString& source) const {
    std::map<QChar, int> counter1;
    std::map<QChar, int> counter2;

    return compare(count(counter1, *target), count(counter2, source));
}
