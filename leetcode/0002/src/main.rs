#[derive(PartialEq, Eq, Clone, Debug)]
pub struct ListNode {
    pub val: i32,
    pub next: Option<Box<ListNode>>
}
//
impl ListNode {
    #[inline]
    pub fn new(val: i32) -> Self {
        ListNode {
            next: None,
            val
        }
    }
}

pub fn vector2list(vec: Vec<i32>) -> Option<Box<ListNode>> {
    let mut result: Option<Box<ListNode>> = None;
    for i in 0..vec.len() {
        let mut new_result = Some(Box::new(ListNode::new(vec[vec.len() - i - 1])));
        new_result.as_mut().unwrap().next = result;
        result = new_result;
    }
    result
}

pub fn list2vector(lst: Option<Box<ListNode>>) -> Vec<i32> {
    let mut result: Vec<i32> = Vec::new();
    let mut cur = &lst;
    loop {
        match cur {
            None => break,
            Some(node) => {
                result.push(node.val);
                cur = &node.next;
            },
        }
    }
    result
}

struct Solution {}

impl Solution {
    pub fn add_two_numbers(l1: Option<Box<ListNode>>, l2: Option<Box<ListNode>>) -> Option<Box<ListNode>> {
        let mut result: Option<Box<ListNode>> = None;
        let mut cur1 = &l1;
        let mut cur2 = &l2;
        let mut carry: i32 = 0;
        {
            let mut cur_res = &mut result;
            loop {
                if *cur1 == None && *cur2 == None && carry == 0 {
                    break;
                } else {
                    let n1 = match cur1 {
                        Some(node) => {
                            cur1 = &cur1.as_ref().unwrap().next;
                            node.val
                        },
                        None => 0
                    };
                    let n2 = match cur2 {
                        Some(node) => {
                            cur2 = &cur2.as_ref().unwrap().next;
                            node.val
                        },
                        None => 0
                    };
                    let n = (n1 + n2 + carry) % 10;
                    carry = if (n1 + n2 + carry) >= 10 { 1 } else { 0 };
                    *cur_res = Some(Box::new(ListNode::new(n)));
                    cur_res = &mut cur_res.as_mut().unwrap().next;
                    continue;
                }
            }
        }
        result
    }
}

fn test(l1: Vec<i32>, l2: Vec<i32>) -> Vec<i32> {
    list2vector(Solution::add_two_numbers(vector2list(l1), vector2list(l2)))
}

fn main() {
    println!("{:?}", test(vec![1,2,3], vec![4,5,6]));
}
