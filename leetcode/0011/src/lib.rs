use std::cmp;

pub struct Solution;

impl Solution {
    pub fn max_area(height: Vec<i32>) -> i32 {
        let hs = |i: isize| -> i32 {
            if i < 0 || i >= height.len() as isize {
                return 0;
            }
            height[i as usize]
        };

        let mut ret: i32 = 0;
        let mut l: isize = -1;
        let mut r: isize = height.len() as isize;
        let mut lh: i32 = 0;
        let mut rh: i32 = 0;

        while l < r {
            if hs(l) > lh {
                lh = hs(l);
            }
            if hs(r) > rh {
                rh = hs(r);
            }
            let hnow = cmp::min(lh, rh);
            let area = hnow * (r - l) as i32;
            if area > ret {
                ret = area;
            }
            if lh < rh {
                l += 1;
            } else {
                r -= 1;
            }
        }
        ret
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn t1() {
        let height = vec![1,8,6,2,5,4,8,3,7];
        assert_eq!(Solution::max_area(height), 49);
    }

    #[test]
    fn t2() {
        let height = vec![1,1];
        assert_eq!(Solution::max_area(height), 1);
    }
}
