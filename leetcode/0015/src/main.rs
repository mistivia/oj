use std::collections::HashMap;
use std::collections::HashSet;

struct Solution {}

fn sort_tuple(t: (i32, i32, i32)) -> (i32, i32, i32) {
    let mut arr = [t.0, t.1, t.2];
    arr.sort();
    (arr[0], arr[1], arr[2])
}

impl Solution {
    pub fn three_sum(nums: Vec<i32>) -> Vec<Vec<i32>> {
        let mut result:Vec<Vec<i32>> = Vec::new();
        let mut result_cache: HashSet<(i32, i32, i32)> = HashSet::new();
        let mut numcache: HashMap<i32, usize> = HashMap::new();
        for i in 0..nums.len() {
            match numcache.get_mut(&nums[i]) {
                Some(val) => {
                    *val = i;
                },
                None => {
                    numcache.insert(nums[i], i);
                }
            }
        }
        for i in 0..(nums.len()-2) {
            for j in i+1..(nums.len()-1) {
                let target = -(nums[i] + nums[j]);
                match numcache.get(&target) {
                    Some(val) => {
                        if *val > j {
                            let key = sort_tuple((nums[i], nums[j], nums[*val]));
                            if !result_cache.contains(&key) {
                                result_cache.insert(key);
                                let (a, b, c) = key;
                                result.push(vec!(a, b, c));
                            }
                        }
                    },
                    None => {}
                }
            }
        }
        return result;
    }
}

pub fn main() {}
