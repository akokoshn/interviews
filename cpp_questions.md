# STL
## MAP
```
int main()
{
  std::map<bool,int> mb = {{1,2},{3,4},{5,0}};
  std::cout << mb.size();
  std::map<int,int> mi = {{1,2},{3,4},{5,0}};
  std::cout << mi.size();
}
```
**`Result: 13`**

For `mb` all 3 initialize elements have same key = 1, because implicit convertion to `bool`.

## VECTOR
```
int main() {
  std::vector<char> delimiters = { ",", ";" };
  std::cout << delimiters[0];
}
```
**`Result: runtime exception`**

Try to apply constructor `template <class InputIterator> vector(InputIterator first, InputIterator last)`, then invalid memory access.
