#pragma once

#include "singlerole.h"
#include "filters/filtercontainer.h"

namespace JApp::Models {

class FilterRole : public SingleRole, public FilterContainer
{
    Q_OBJECT
    Q_INTERFACES(JApp::Models::FilterContainer)
    Q_PROPERTY(QQmlListProperty<JApp::Models::Filter> filters READ filtersListProperty)
    Q_CLASSINFO("DefaultProperty", "filters")

public:
    using SingleRole::SingleRole;

private:
    void onFilterAppended(Filter* filter) override;
    void onFilterRemoved(Filter* filter) override;
    void onFiltersCleared() override;

    QVariant data(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel) override;
};

}
