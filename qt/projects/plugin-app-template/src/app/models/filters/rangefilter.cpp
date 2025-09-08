#include "rangefilter.h"
#include <JApp/Log.h>

using namespace JApp::Models;

/*!
    \qmltype RangeFilter
    \inherits RoleFilter
    \inqmlmodule SortFilterProxyModel
    \ingroup Filters
    \brief Filters rows between boundary values.

    A RangeFilter is a \l RoleFilter that accepts rows if their data is between the filter's minimum and maximum value.

    In the following example, only rows with their \c price role set to a value between the tow boundary of the slider will be accepted :
    \code
    RangeSlider {
       id: priceRangeSlider
    }

    SortFilterProxyModel {
       sourceModel: priceModel
       filters: RangeFilter {
           roleName: "price"
           minimumValue: priceRangeSlider.first.value
           maximumValue: priceRangeSlider.second.value
       }
    }
    \endcode
*/

/*!
    \qmlproperty int RangeFilter::minimumValue

    This property holds the minimumValue of the filter.
    Rows with a value lower than \c minimumValue will be rejected.

    By default, no value is set.

    \sa minimumInclusive
*/
QVariant RangeFilter::minimumValue() const
{
    return m_minimumValue;
}

void RangeFilter::setMinimumValue(QVariant minimumValue)
{
    if (!minimumValue.isValid())
    {
        LOG_WARN() << "Can't set minimum value, given value is invalid: " << minimumValue;
        return;
    }

    if (m_minimumValue == minimumValue)
        return;

    m_minimumValue = minimumValue;
    Q_EMIT minimumValueChanged();
    invalidate();
}

/*!
    \qmlproperty int RangeFilter::minimumInclusive

    This property holds whether the \l minimumValue is inclusive.

    By default, the \l minimumValue is inclusive.

    \sa minimumValue
*/
bool RangeFilter::minimumInclusive() const
{
    return m_minimumInclusive;
}

void RangeFilter::setMinimumInclusive(bool minimumInclusive)
{
    if (m_minimumInclusive == minimumInclusive)
        return;

    m_minimumInclusive = minimumInclusive;
    Q_EMIT minimumInclusiveChanged();
    invalidate();
}

/*!
    \qmlproperty int RangeFilter::maximumValue

    This property holds the maximumValue of the filter.
    Rows with a value higher than \c maximumValue will be rejected.

    By default, no value is set.

    \sa maximumInclusive
*/
QVariant RangeFilter::maximumValue() const
{
    return m_maximumValue;
}

void RangeFilter::setMaximumValue(QVariant maximumValue)
{
    if (!maximumValue.isValid())
    {
        LOG_WARN() << "Can't set maximum value, given value is invalid: " << maximumValue;
        return;
    }

    if (m_maximumValue == maximumValue)
        return;

    m_maximumValue = maximumValue;
    Q_EMIT maximumValueChanged();
    invalidate();
}

/*!
    \qmlproperty int RangeFilter::maximumInclusive

    This property holds whether the \l minimumValue is inclusive.

    By default, the \l minimumValue is inclusive.

    \sa minimumValue
*/
bool RangeFilter::maximumInclusive() const
{
    return m_maximumInclusive;
}

void RangeFilter::setMaximumInclusive(bool maximumInclusive)
{
    if (m_maximumInclusive == maximumInclusive)
        return;

    m_maximumInclusive = maximumInclusive;
    Q_EMIT maximumInclusiveChanged();
    invalidate();
}

bool RangeFilter::filterRow(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel) const
{
    const QVariant value = sourceData(sourceIndex, proxyModel);
    QPartialOrdering minComparisonResult = QVariant::compare(value, m_minimumValue);
    QPartialOrdering maxComparisonResult = QVariant::compare(m_maximumValue, value);

    if (minComparisonResult == QPartialOrdering::Unordered)
    {
        LOG_WARN() << "Failed to filter row with value " << value << ", comparison failed with minimum value " << m_minimumValue;
    }
    if (maxComparisonResult == QPartialOrdering::Unordered)
    {
        LOG_WARN() << "Failed to filter row with value " << value << ", comparison failed with minimum value " << m_minimumValue;
    }

    bool isLessThanMin = m_minimumInclusive ? (minComparisonResult == QPartialOrdering::Equivalent || minComparisonResult == QPartialOrdering::Less) : minComparisonResult == QPartialOrdering::Less;
    bool isGreaterThanMax = m_maximumInclusive ? (maxComparisonResult == QPartialOrdering::Equivalent || minComparisonResult == QPartialOrdering::Greater) : minComparisonResult == QPartialOrdering::Greater;

    return !(isLessThanMin || isGreaterThanMax);
}
