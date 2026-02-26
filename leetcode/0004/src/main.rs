use std::cmp::{min, max};

impl Solution {
    fn vec_get(v: &Vec<i32>, n: i32) -> i32 {
        if n < 0 {
            return i32::MIN;
        }
        if n >= v.len() as i32 {
            return i32::MAX;
        }
        return v[n as usize];
    }
    pub fn find_median_sorted_arrays(nums1: Vec<i32>, nums2: Vec<i32>) -> f64 {
        let mut n1 = &nums1;
        let mut n2 = &nums2;
        if n1.len() < n2.len() {
            n1 = &nums2;
            n2 = &nums1;
        }

        if n2.len() == 0 || n1[n1.len()-1] <= n2[0] {
            if (n1.len() + n2.len()) % 2 == 0 {
                let mid_index = (n1.len() + n2.len()) / 2;
                let right: i32;
                if mid_index == n1.len() {
                    right = n2[0];
                } else {
                    right = n1[mid_index];
                }
                return (n1[mid_index - 1] + right) as f64 / 2.0;
            } else {
                let mid_index = (n1.len() + n2.len()) / 2;
                return n1[mid_index] as f64;
            }
        }
        if n2[n2.len()-1] <= n1[0] {
            if (n1.len() + n2.len()) % 2 == 0 {
                let mid_index = (n1.len() + n2.len()) / 2 - n2.len();
                let left: i32;
                if mid_index <= 0 {
                    left = n2[n2.len() - 1];
                } else {
                    left = n1[mid_index - 1];
                }
                return (left + n1[mid_index]) as f64 / 2.0;
            } else {
                let mid_index = (n1.len() + n2.len()) / 2 - n2.len();
                return n1[mid_index] as f64;
            }
        }

        let mut left: i32 = 0;
        let mut right: i32 = n1.len() as i32 - 1;
        let mut mid: i32 = 0;
        let mut mid2: i32 = 0;
        loop {
            if left > right { break; }
            mid = (left + right) / 2;
            mid2 = (n1.len() + n2.len()) as i32 / 2 - (1 + mid) - 1;
            if mid2 < -1 {
                right = mid - 1;
                continue;
            }
            if mid2 >= n2.len() as i32 {
                left = mid + 1;
                continue;
            }
            if n1[mid as usize] > Self::vec_get(n2, mid2 + 1) {
                right = mid - 1;
                continue;
            }
            if n1[mid as usize + 1] < Self::vec_get(n2, mid2) {
                left = mid + 1;
                continue;
            }
            break;
        }
        if (n1.len() + n2.len()) % 2 == 0 {
            (max(n1[mid as usize], Self::vec_get(n2, mid2))
                + min(n1[mid as usize + 1], Self::vec_get(n2, mid2 + 1))) as f64  / 2.0
        } else {
            min(n1[mid as usize +1], Self::vec_get(n2, mid2 + 1)) as f64
        }
    }
}

struct Solution {}
fn main() {
    println!("{}", Solution::find_median_sorted_arrays(vec![1,2,3], vec![4,5,6]));
    println!("{}", Solution::find_median_sorted_arrays(vec![4,5,6], vec![1,2,3]));
    println!("{}", Solution::find_median_sorted_arrays(vec![1,2,3], vec![5,6]));
    println!("{}", Solution::find_median_sorted_arrays(vec![4,5,6], vec![1,2]));
    println!("{}", Solution::find_median_sorted_arrays(vec![1,3,5], vec![2,4,6]));
    println!("{}", Solution::find_median_sorted_arrays(vec![3], vec![1,2,4,5]));
}