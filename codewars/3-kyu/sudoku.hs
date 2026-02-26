-- https://www.codewars.com/kata/5296bc77afba8baa690002d7
module Sudoku where

import Data.List (find, transpose)

sudoku :: [[Int]] -> [[Int]]
sudoku input = stripMaybe $ fmap (chunksOf 9) $ solveSudoku $ foldl1 (++) $ input where
  stripMaybe Nothing = []
  stripMaybe (Just x) = x
  

chunksOf :: Int -> [a] -> [[a]]
chunksOf n lst = go lst [] where
  go [] ret = reverse ret
  go rem ret = go (drop n rem) ((take n rem):ret)

type ListZipper a = ([a], [a])

moveForward :: ListZipper a -> Maybe (ListZipper a)
moveForward (_, []) = Nothing
moveForward (before, (x:xs)) = Just ((x:before), xs)

toList :: ListZipper a -> [a]
toList (before, after) = reverse before ++ after

fromList :: [a] -> ListZipper a
fromList lst = ([], lst)

zipperModify :: a -> ListZipper a -> Maybe (ListZipper a)
zipperModify _ (b, []) = Nothing
zipperModify new (before, (x:xs)) = Just (before, (new:xs))

zipperView :: ListZipper a -> Maybe a
zipperView (_, []) = Nothing
zipperView (_, (x:_)) = Just x

firstJust :: [Maybe a] -> Maybe a
firstJust lst = getResult $ find isJust lst where
    getResult (Just (Just x)) = Just x
    getResult _ = Nothing


isJust :: Maybe a -> Bool
isJust (Just _) = True
isJust Nothing  = False

solveSudoku :: [Int] -> Maybe [Int]
solveSudoku s = go $ fromList s where
  go z =
    if not $ maybeValidSudoku $ toList z then Nothing
    else
      if moveForward z == Nothing then
          Just $ toList z
      else
        if zipperView z /= Just 0 then moveForward z >>= go
        else
          firstJust $ map tryWith [1,2,3,4,5,6,7,8,9] where
           tryWith x = zipperModify x z >>= moveForward >>= go
        
maybeValidSudoku :: [Int] -> Bool
maybeValidSudoku x = all validLine (rows x ++ cols x ++ blocks x) where
  validLine line = all isUnique [1,2,3,4,5,6,7,8,9] where
    isUnique x = (length $ filter (==x) line) <= 1

rows :: [Int] -> [[Int]]
rows lst = chunksOf 9 lst

cols :: [Int] -> [[Int]]
cols lst = transpose $ chunksOf 9 lst

blocks :: [Int] -> [[Int]]
blocks lst = map block ([(rows, cols) | rows <- ranges, cols <- ranges]) where
  ranges = [(0,2), (3,5), (6,8)]
  block ((startx, endx), (starty, endy)) =
    foldl1 (++)
    $ colsRange startx endx
    $ rowsRange starty endy
    $ chunksOf 9 lst where
      rowsRange start end mat = drop start $ take (end+1) mat
      colsRange start end mat = transpose $ drop start $ take (end+1) $ transpose mat
