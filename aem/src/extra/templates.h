/* This will make it easy to create a struct if it doesn't already exist */
template<class T>
T* find_or_create(vector<T> *list, T *newT) {
  int i_size = list->size();
  for (int i=0; i<i_size; i++) {
    if ((*list)[i].id == newT->id) return &(*list)[i];
  }

  list->push_back(*newT);
  return &list->back();
}

