#ifndef SPDIAGNOSTICISSUESLIST_H
#define SPDIAGNOSTICISSUESLIST_H

#include <vector>
#include <memory>

using namespace std;

template <class T>
class SPList
{
public:
    SPList();

    void add(shared_ptr<T> issue);
    shared_ptr<T> issueAt(int index);
    int  count();

private:
    vector<shared_ptr<T> > issues;
};

template <class T>
SPList<T>::SPList()
{

}

template <class T>
void SPList<T>::add(shared_ptr<T> issue) {

    issues.push_back(issue);

}

template <class T>
shared_ptr<T> SPList<T>::issueAt(int index) {

    if (index < 0 || index >= count()) {

        return shared_ptr<T>();

    }

    return issues[index];

}

template <class T>
int SPList<T>::count() {

    return issues.size();

}

#endif // SPDIAGNOSTICISSUESLIST_H
