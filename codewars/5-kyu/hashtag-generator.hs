-- https://www.codewars.com/kata/52449b062fb80683ec000024

module Codewars.Kata.Hashtag where

import Data.Char
import Control.Monad

capitalized w = (toUpper $ head w) : (map toLower $ tail w)

processCapChar s =
  join $ map (\x -> if isLower x then [x] else " "++[x]) s

generateHashtag :: String -> Maybe String
generateHashtag s = impl (processCapChar s)
  where
    impl s
      | length (words s) == 0 = Nothing
      | otherwise =
        let result = "#" ++ (join $ map capitalized $ words s)
        in
          if (length result) > 140 then
            Nothing
          else
            Just result
