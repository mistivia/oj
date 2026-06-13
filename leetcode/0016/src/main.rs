struct Solution {}

impl Solution {
    pub fn three_sum_closest(nums: Vec<i32>, target: i32) -> i32 {
        let mut delta = i32::MAX;
        let mut result = 0;
        let mut mnums = nums;
        mnums.sort();
        let len = mnums.len();
        for i in 0..len-2 {
            let mut j = i + 1;
            let mut k = len - 1;
            while j < k {
                let sum = mnums[i] + mnums[j] + mnums[k];
                if (sum - target).abs() < delta {
                    delta = (sum - target).abs();
                    result = sum;
                }
                if sum - target > 0 {
                    k = k - 1;
                } else {
                    j = j + 1;
                }
            }
        }
        result
    }
}

fn main() { }
