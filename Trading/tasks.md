# SKANESTAS
## OrderBook
Enable class `OredrBook`
```
QuoteUpdate
{
    uint64_t timestamp;
    string side; //bid or ask
    uint64_t price;
    uint64_t volume;
}
class OrderBook
{
  public:
    void add(const QuoteUpdate& update);
    int best_bid();
    int best_ask();
    int spread(); // best_ask − best_bid
};
```
