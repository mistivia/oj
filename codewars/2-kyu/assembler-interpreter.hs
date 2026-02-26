-- https://www.codewars.com/kata/58e61f3d8ff24f774400002c
module AssemblerInterpreter where

import Debug.Trace

import Data.Function((&))
import Data.Maybe (fromMaybe)
import Text.Parsec hiding (State)
import Text.Parsec.Char
import qualified Data.Map as M

import Control.Monad.Trans.Maybe
import Control.Monad.Trans.State
import Control.Monad.Trans.Class(lift)

type Parser = Parsec String ()

data Op =
  Mov | Inc | Dec | Add | Sub  | Mul | Div | Jmp | Cmp | Jne | Je |
  Jge | Jg  | Jle | Jl  | Call | Ret | Msg | End 
  deriving (Show)

stringP :: String -> Parser String
stringP str = try $ string str

opStrP :: Parser String
opStrP = choice $ map stringP [
  "mov", "inc", "dec", "add", "sub", "mul", "div", "jmp", "cmp", "jne",
  "je", "jge", "jg", "jle", "jl", "call", "ret", "msg", "end"]

opP :: Parser Op
opP = do
  s <- opStrP
  case s of
    "mov" -> return Mov
    "inc" -> return Inc
    "dec" -> return Dec
    "add" -> return Add
    "sub" -> return Sub
    "mul" -> return Mul
    "div" -> return Div
    "jmp" -> return Jmp
    "cmp" -> return Cmp
    "jne" -> return Jne
    "je"  -> return Je
    "jge" -> return Jge
    "jg"  -> return Jg
    "jle" -> return Jle
    "jl"  -> return Jl
    "call"-> return Call
    "ret" -> return Ret
    "msg" -> return Msg
    "end" -> return End

newtype Label = Label String
  deriving (Show)

inlineSpace :: Parser Char
inlineSpace = oneOf [' ', '\t'] 
inlineSpaces = skipMany $ oneOf [' ', '\t'] 

identifierP :: Parser String
identifierP = do
  notFollowedBy opStrP
  x <- letter <|> oneOf ['_']
  xs <- many (alphaNum <|> oneOf ['_'])
  return (x:xs)

labelP :: Parser Label
labelP = do
  id <- identifierP
  oneOf [':']
  return $ Label id

data Stmt = LabelStmt Label | InstrStmt Op [Arg]
  deriving (Show)

data Arg = IdentifierArg String | NumberArg Int | StringArg String
  deriving (Show)

argP :: Parser Arg
argP = do
  inlineSpaces
  let identifierArgP = IdentifierArg <$> identifierP
      numberP = do
        negSign <- optionMaybe $ char '-'
        let isNeg = case negSign of
              Just _ -> True
              Nothing -> False
        digits <- many1 digit
        let n :: Int = read digits 
        return $ NumberArg $ if isNeg then
          negate n
        else n
      stringP = do
        oneOf ['\''] :: Parser Char
        str <- many $ noneOf ['\'']
        oneOf ['\'']
        return $ StringArg str
  arg <- identifierArgP <|> numberP <|> stringP
  inlineSpaces
  return arg

stmtP :: Parser Stmt
stmtP = do
  let
    labelStmtP = LabelStmt <$> labelP
    argsP = do
      inlineSpaces
      oneOf [',']
      inlineSpaces
      arg <- argP
      inlineSpaces
      return arg
    instrStmtP = do
      op <- opP
      inlineSpaces
      marg <- optionMaybe argP
      args <- case marg of
        Just arg -> do
          targs <- many argsP
          return (arg:targs)
        Nothing -> return []
      return $ InstrStmt op args
  inlineSpaces
  stmt <- labelStmtP <|> instrStmtP
  inlineSpaces
  return stmt

commentP :: Parser ()
commentP = do
  oneOf [';']
  many $ noneOf ['\n']
  return ()

stmtLineP :: Parser [Stmt]
stmtLineP = do
  inlineSpaces
  ms <- optionMaybe stmtP
  inlineSpaces
  optional commentP
  inlineSpaces
  case ms of
    Just s -> return [s]
    Nothing -> return []

progP :: Parser [Stmt]
progP = do
  let newlineP = do
        newline :: Parser Char
        return (++)
  stmts <- chainl stmtLineP newlineP []
  spaces
  eof
  return stmts

processLabel :: [Stmt] -> ([Stmt], M.Map String Int)
processLabel stmts = go stmts [] M.empty 0 where
  go [] processed labelMap i = (reverse processed, labelMap)
  go (instr@(InstrStmt op args):xs) processed labelMap i =
    go xs (instr:processed) labelMap (i+1)
  go ((LabelStmt (Label ident)):xs) processed labelMap i =
    let newLabelMap = M.insert ident i labelMap 
    in
      go xs processed newLabelMap i

data MachineState = MachineState
  { machineRegisters :: M.Map String Int
  , machineStack :: [Int]
  , machineProgCnt :: Int
  , machineOutput :: Maybe String
  , machineProg :: [Stmt]
  , machineLabels :: M.Map String Int
  , machineCmpFlag :: Ordering
  , machineEnd :: Bool
  }
  deriving (Show)

type MachineM = MaybeT (State MachineState)

nextInstr :: MachineM ()
nextInstr = do
  ms <- lift get
  lift $ put (ms {machineProgCnt = machineProgCnt ms + 1})

fetchInstr :: MachineM (Maybe Stmt)
fetchInstr = do
  ms <- lift get
  let pc = machineProgCnt ms
      prog = machineProg ms
  if pc >= length prog || pc < 0 || machineEnd ms then return Nothing
  else return $ Just (prog !! pc)

setReg :: String -> Int -> MachineM ()
setReg r n = do
  ms <- lift get
  let regs = machineRegisters ms
  lift $ put ms {machineRegisters = M.insert r n regs}

getReg :: String -> MachineM Int
getReg r = do
  ms <- lift get
  let regs = machineRegisters ms
  case M.lookup r regs of
    Just n -> return n
    Nothing -> fail []

machinePush :: Int -> MachineM ()
machinePush n = do
  ms <- lift get
  let stack = machineStack ms
  lift $ put ms {machineStack = n:stack}

machinePop :: MachineM Int
machinePop = do
  ms <- lift get
  case machineStack ms of
    [] -> fail []
    (x:xs) -> do
      lift $ put ms {machineStack = xs}
      return x

execInstr :: Stmt -> MachineM ()
execInstr (InstrStmt op args) = decodeOp op args
execInstr _ = fail []

getResult :: MachineM String
getResult = do
  ms <- lift get
  if not (machineEnd ms) then fail []
  else case machineOutput ms of
    Nothing -> fail []
    Just msg -> return msg

runMachine :: MachineM String
runMachine = do
  ms <- lift get
  instr <- fetchInstr
  case instr of
    Nothing -> getResult
    Just stmt -> do execInstr stmt ; runMachine

execMov :: [Arg] -> MachineM ()
execMov [IdentifierArg reg, NumberArg n] = do
  setReg reg n
  nextInstr
execMov [IdentifierArg r1, IdentifierArg r2] = do
  n <- getReg r2
  setReg r1 n
  nextInstr
execMov _ = fail []

execMsg :: [Arg] -> MachineM ()
execMsg [] = fail []
execMsg args = go args "" where
  go [] msg = do
    ms <- lift get
    lift $ put ms { machineOutput = Just msg }
    nextInstr
  go ((NumberArg n):xs) msg = go  xs (msg ++ show n)
  go ((StringArg s):xs) msg = go xs (msg ++ s)
  go ((IdentifierArg reg):xs) msg = do
    n <- getReg reg
    go xs (msg ++ show n)

execEnd :: [Arg] -> MachineM ()
execEnd [] = do
  ms <- lift get
  lift $ put ms { machineEnd = True }
execEnd _ = fail []

execInc :: [Arg] -> MachineM()
execInc [IdentifierArg r] = do
  n <- getReg r
  setReg r (n+1)
  nextInstr
execInc _ = fail []

execDec :: [Arg] -> MachineM()
execDec [IdentifierArg r] = do
  n <- getReg r
  setReg r (n-1)
  nextInstr
execDec _ = fail []

execArithmetic :: (Int->Int->Int) -> [Arg] -> MachineM ()
execArithmetic op [IdentifierArg ra, IdentifierArg rb] = do
  n <- getReg rb
  execArithmetic op [IdentifierArg ra, NumberArg n]
execArithmetic op [IdentifierArg r, NumberArg n] = do
  x <- getReg r
  setReg r (op x n) 
  nextInstr
execArithmetic _ _ = fail []

execAdd = execArithmetic (+)
execSub = execArithmetic (-)
execMul = execArithmetic (*)
execDiv = execArithmetic div

machineGoto :: Int -> MachineM ()
machineGoto p = do
  ms <- lift get
  lift $ put ms {machineProgCnt = p}

execJmp :: [Arg] -> MachineM ()
execJmp [IdentifierArg lbl] = do
  ms <- lift get
  let labelMap = machineLabels ms
  case M.lookup lbl labelMap of
    Just pos -> machineGoto pos
    Nothing -> fail []
execJmp _ = fail []

execCmp :: [Arg] -> MachineM ()
execCmp [IdentifierArg r1, a2] = do
  n1 <- getReg r1
  execCmp [NumberArg n1, a2]
execCmp [a1, IdentifierArg r2] = do
  n2 <- getReg r2
  execCmp [a1, NumberArg n2]
execCmp [NumberArg n1, NumberArg n2] = do
  ms <- lift get
  lift $ put ms { machineCmpFlag = compare n1 n2 }
  nextInstr

execCondJmp :: [Ordering] -> [Arg] -> MachineM ()
execCondJmp conds [IdentifierArg lbl] = do
  flag <- machineCmpFlag <$> lift get
  if flag `elem` conds then execJmp [IdentifierArg lbl]
  else nextInstr
execCondJmp conds _ = fail []

execCall :: [Arg] -> MachineM ()
execCall [arg@(IdentifierArg lbl)] = do
  ms <- lift get
  let p = machineProgCnt ms + 1
  machinePush p
  execJmp [arg]

execRet :: [Arg] -> MachineM ()
execRet [] = do
  n <- machinePop
  machineGoto n
execRet _ = fail []

decodeOp :: Op -> [Arg] -> MachineM ()
decodeOp Mov = execMov
decodeOp Inc = execInc
decodeOp Dec = execDec
decodeOp Add = execAdd
decodeOp Sub = execSub
decodeOp Mul = execMul
decodeOp Div = execDiv
decodeOp Jmp = execJmp
decodeOp Cmp = execCmp
decodeOp Jl = execCondJmp [LT]
decodeOp Jg = execCondJmp [GT]
decodeOp Je = execCondJmp [EQ]
decodeOp Jge = execCondJmp [EQ, GT]
decodeOp Jle = execCondJmp [EQ, LT]
decodeOp Jne = execCondJmp [GT, LT]
decodeOp Call = execCall
decodeOp Ret = execRet
decodeOp Msg = execMsg
decodeOp End = execEnd


buildMachine :: String -> Maybe MachineState
buildMachine code = 
  let
    parseResult = runParser progP () "" code
  in
    case parseResult of
      Right lines ->
        let (stmts, labels) = processLabel lines
        in
          Just MachineState
            { machineRegisters = M.empty
            , machineEnd = False
            , machineLabels = labels
            , machineOutput = Nothing
            , machineProg = stmts
            , machineProgCnt = 0
            , machineCmpFlag = EQ
            , machineStack = []
            }
      _ -> Nothing


interpret :: String -> Maybe String
interpret code = do
  ms <- buildMachine code
  let (res, _) = runState (runMaybeT runMachine) ms
  res
