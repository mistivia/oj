use std::collections::HashMap;

struct Solution {}
impl Solution {
    pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
        let mut num_map : HashMap<i32, usize> = HashMap::new();

        let mut i:usize = 0;
        loop {
            let index = target - nums[i];
            match num_map.get(&index) {
                Some(j) => break vec![i as i32, *j as i32],
                None => {
                    num_map.insert(nums[i], i);
                    i = i + 1;
                    continue;
                }
            }
        }
    }
}

fn main() {
    println!("{:?}", Solution::two_sum(vec![2, 7, 11, 15], 9));
}