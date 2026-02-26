-- https://www.codewars.com/kata/52742f58faf5485cae000b9a
module FormatDuration where

import Data.List

units = ["second", "minute", "hour", "day", "year"]

unitsLen :: (Integral i) => [i]
unitsLen = [60, 60, 24, 365]

seris [] cur ret = reverse (cur:ret)
seris (x:xs) cur ret = seris xs (cur `div` x) ((cur `mod` x):ret)

zipUnit nums = filter (/="") $ zipWith fn nums units
  where
    fn 0 unit = ""
    fn 1 unit = "1 " ++ unit
    fn n unit = (show n) ++ " " ++ unit ++ "s"
                
joinUnits :: [String] -> String
joinUnits [x] = x
joinUnits (x:xs) = (intercalate ", " $ reverse xs) ++ " and " ++ x

formatDuration :: (Integral i, Show i) => i -> String
formatDuration n = if toInteger n == 0 then "now"
                   else joinUnits $ zipUnit $ seris unitsLen n []
