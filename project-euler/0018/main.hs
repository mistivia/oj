import Text.Parsec
import Text.Parsec.String (Parser, parseFromFile)
import qualified Data.Map.Strict as Map
import Control.Monad.ST
import Data.STRef
import Data.Vector (Vector)
import qualified Data.Vector as V

int :: Parser Int
int = read <$> many1 digit

line :: Parser [Int]
line = sepBy1 int (char ' ')

numTriangle :: Parser [[Int]]
numTriangle = do
    triag <- sepEndBy1 line endOfLine
    eof
    pure triag

numAt :: Vector (Vector Int) -> Int -> Int -> Int
numAt triag row col = triag V.! row V.! col

maxSum :: Vector (Vector Int) -> Int
maxSum triag = runST $ do
    mref <- newSTRef Map.empty
    let n = V.length triag
    let go r c = do
            m <- readSTRef mref
            case Map.lookup (r, c) m of
                Just v -> pure v
                Nothing -> do
                    let val = numAt triag r c
                    res <- if r == n - 1
                            then pure val
                            else do
                                left <- go (r + 1) c
                                right <- go (r + 1) (c + 1)
                                pure (val + max left right)
                    writeSTRef mref (Map.insert (r, c) res m)
                    pure res
    go 0 0

listToVec :: [[Int]] -> Vector (Vector Int)
listToVec = V.fromList . map V.fromList

main = do
    result <- parseFromFile numTriangle "input"
    case result of
        Left err -> print err
        Right triag -> print $ maxSum $ listToVec triag
