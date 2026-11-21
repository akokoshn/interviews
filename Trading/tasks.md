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
## Input example
| Price | Volume | Side |
|:-----:|:------:|:-----|
|150.25 | 30 | Ask |
|150.20 | 80 | Ask |
|150.15 | 45 | Ask |
|150.10 | 50 | Bid |
|150.05 | 120 | Bid |
|150.00 | 200 | Bid |

Best bid = 150.10, best ask = 150.15

## Window statistics
Suggest improvements for the follow implementation of the class which collects statistic by fixed size sliding window of incoming prices
```
struct RollingStats {
    window_size: usize,// useless
    values: Vec<f64>,
}

impl RollingStats {
    fn new(window_size: usize) -> Self {
        RollingStats { window_size, values: Vec::new() }
    }

    fn add(&mut self, value: f64) {
        
        if self.values.len() >= self.window_size {
            self.values.remove(0);
        }
        self.values.push(value);
    }

    fn mean(&self) -> f64 {
        let sum: f64 = self.values.iter().sum();
        sum / self.values.len() as f64
    }

    fn stddev(&self) -> f64 {
        let mean = self.mean();
        let variance = self.values.iter()
            .map(|v| (v - mean).powi(2))
            .sum::<f64>() / self.values.len() as f64;
        variance.sqrt()
    }

    fn median(&self) -> f64 {
        let mut sorted = self.values.clone();
        sorted.sort_by(|a, b| {
            a.partial_cmp(b).unwrap()
        });
        sorted[sorted.len() / 2]
    }
}
```
