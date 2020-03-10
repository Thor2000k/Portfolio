---
toc: 1
numbersections: true
geometry: margin=2.5cm
title: Programming Languages (Project 1)
author: Thor Skjold Haagensen (thhaa16)
date: November 13, 2017
abstract: |
    The goal of this project is ...
---


\newpage


The Kalaha game with parameters $(n,m)$
====
import Data.list

\begin{code}
module Kalaha where

import Data.List
type PitCount   = Int
type StoneCount = Int
data Kalaha     = Kalaha PitCount StoneCount deriving (Show, Read, Eq)

type KPos       = Int
type KState     = [Int]
type Player     = Bool
\end{code}


The function `startStateImpl`

This function is responsible for initializing a kalaha game, its output is a list of the size 2 * n + 2 where index ( n ) and ( 2 * n + 1 ) have the value 0 and the rest have the value m. 
This is achieved by first repeating m n times afterwards adding 0 to the front of the list and afterwards reversing the list. This list is concatinated with a list of similar properties. 
----

\begin{code}
startStateImpl :: Kalaha -> KState
startStateImpl k@(Kalaha n m) = reverse ( 0 : take n (repeat m)) ++ reverse ( 0 : take n (repeat m))
\end{code}


The function `movesImpl`

This method returns a list of moves for either player True or False. The moves correspond to indices of nonempty pits in the list. This is achieved by using the method findIndices, which is part of the data.List package. findIndices takes a predicate and a list, in my implementation the predicate is all values larger than 0 and for player false the first n numbers of KState are taken and for player True we first drop n + 1 numbers afterwards taking n.
----

\begin{code}
movesImpl :: Kalaha -> Player -> KState -> [KPos]
movesImpl k@(Kalaha n m) p s 
    |p == False = findIndices(>0) (take n s)
    |p == True = findIndices(>0) (take n (drop (n+1) s))


\end{code}


The function `valueImpl`

This function returns player true - player falses kalaha pits effectivly returning the score of the game. This is achieved by subtracting the value on index 2*n-1 with the value on index n, since the values store in the indices are integers we use the fromIntegral function to convert it into a double. 
----

\begin{code}
valueImpl :: Kalaha -> KState -> Double
valueImpl k@(Kalaha n m) s = fromIntegral ((s !! (2*n + 1)) - (s !! n))
\end{code}

The function `moveImpl`

This function is responsible for making a move and finding out who the next player is. This is achieved by using a lot of helper functions. 
moveImpl calls moveHelper with a Kalaha and a Player, however instead of the  current Kstate it calls takeStones on the Kstate recieved in moveImpl, takeStones returns a state where the index of KPos has been set to 0. moveHelper then gets KPos + 1 and the number of stones at index KPos. 
----

\begin{code}
moveImpl :: Kalaha -> Player -> KState -> KPos -> (Player,KState)
moveImpl k@(Kalaha n m) p s xs = moveHelper k p (takeStones xs s) (xs+1) (s!!xs) 
\end{code}


The function `moveHelper`

moveHelper has 5 cases. The first checks if the number of stones to be sowed has reached 0, if that is the case the method calls the function nextPlayer. nextPlayer takes the same variables as moveImpl and when called recieves the same parameters but with KPos - 1. The reason for the -1 is that we allready move to the next index in the list when it is called so we have to move one back. 

Case 2 and 3 checks that the players dont put stones in the opponents kalaha, this is done by checking whether KPos is equal to the index of the opponents kalaha, (n - 1) for player false and ( 2 * n + 1) for player true. 

case 4 
----
\begin{code}
moveHelper :: Kalaha -> Player -> KState -> KPos -> Int -> (Player,KState)
moveHelper k@(Kalaha n m) p s xs stones
                    | stones == 0 = nextPlayer k p s (xs-1)
                    | p == False && xs == (2 * n + 1)= moveHelper k p s 0 stones
                    | p == True && xs == (n - 1) =  moveHelper k p s (xs + 1) stones
                    | xs <= 2 * n + 1  = moveHelper k p (makeMove xs s) (xs + 1) (stones - 1)
                    | xs > 2 * n + 1 = moveHelper k p s 0 stones 

 
\end{code}

The function `makeMove`
----
This function adds 1 to a given index xs 

\begin{code}
makeMove :: KPos -> KState -> KState 
makeMove xs s = (fst( splitAt xs s)) ++ [(s!!xs) + 1] ++ (snd( splitAt (xs + 1) s ))


\end{code}

The function `takeStones`
----
This function removes all the stones from a pit 

\begin{code}

takeStones :: KPos -> KState -> KState   
takeStones xs s = (fst( splitAt xs s)) ++ [0] ++ (snd( splitAt (xs + 1) s ))

\end{code}

The function `nextPlayer`
----
This function checks the different cases to figure out which players turn it is 
\begin{code}
nextPlayer :: Kalaha -> Player -> KState -> KPos -> (Player, KState)
nextPlayer k@(Kalaha n m) p s xs 
        | movesImpl k True s == [] = ( False, (fst (splitAt(n + 1) s) ++ take n (repeat 0 )) ++ [s!!(2 * n + 1) + (sum (init (snd (splitAt (n + 1) s))))] ) 
        | movesImpl k False s == [] = (True, take n (repeat 0) ++ [s!!(n) + sum(fst(splitAt(n) s))] ++ snd(splitAt (n + 1) s))
        | p == False && xs == (n) = (p, s)
        | p == True && xs == (2 * n + 1) = (p,s)
        | p == False && xs < n && (s!!xs) == 1 =  ( True, npHelper k p s xs )
        | p == True && xs > n && xs < (2 * n + 1) && (s!!xs) == 1 = ( False, npHelper k p s xs )
        | otherwise = (,) (not p) s

\end{code}

The function `npHelper`
----
\begin{code}

npHelper :: Kalaha -> Player -> KState -> KPos -> KState
npHelper k@(Kalaha n m) p s xs
    | p == False = (takeStones xs (fst(splitAt n s))) ++ [(s!!n) + (s !! ((2*n) - xs)) + 1] ++ (snd(splitAt (n + 1) (takeStones (2*n-xs) s)))
    | otherwise =  (takeStones (2*n - (xs)) (fst(splitAt (xs ) s))) ++ init (snd(splitAt (xs) (takeStones (xs) s))) ++ [s!!(2*n + 1) + 1 + (s!!((2 * n) - (xs)))]  

\end{code}



The function `showGameImpl`
----

\begin{code}
showGameImpl :: Kalaha -> KState -> String
showGameImpl g@(Kalaha n m) xs = printTop g ( reverse ( take n ( drop (n+1) (xs)))) ++ printMid g ([xs!!n]++[last ( xs )]) ++ printTop g (drop (n + 1) (init(xs)))
\end{code}

The function `printTop`
-----
\begin{code}
printTop :: Kalaha -> KState -> String
printTop g@(Kalaha n m) (x:xs) 
    | length ( (x:xs) ) == 1 = (concat (replicate ( (maxLen + 1) - (length (show( x )))) " ")) ++ ( show x ) ++ "\n"
    | length ( (x:xs) )  == n  = (concat (replicate (maxLen + 1) (" "))) ++ (concat (replicate ( (maxLen + 1) - (length (show( x )))) " ")) ++ ( show x ) ++ (printTop g (xs)) 
    | otherwise = (concat (replicate ( (maxLen + 1) - (length (show( x )))) " ")) ++ ( show x ) ++ ( printTop g xs ) 
        where maxLen = length ( show( 2 * n * m))

\end{code}


The function `printMid`
----
\begin{code}
printMid :: Kalaha -> KState -> String
printMid g@(Kalaha n m) xs =
    let maxLen = length ( show( 2 * n * m)) 
    in (concat (replicate ((maxLen + 1) - (length( show( head( xs ))))) (" ") ))  ++  show ( head (xs) ) ++ (concat( replicate (n ) (concat (replicate (maxLen + 1) " ")))) ++ (concat (replicate ((maxLen + 1) - (length( show( last( xs ))))) (" ") ))  ++  show ( last (xs) ) ++ "\n"

\end{code}


Trees
====

\begin{code}
data Tree m v  = Node v [(m,Tree m v)] deriving (Eq, Show)
\end{code}

The function `takeTree`
----

\begin{code}
takeTree :: Int -> Tree m v -> Tree m v
takeTree i t@(Node v q) 
    | i == 0 = Node v []
    | otherwise = Node v [ (fst x , takeTree (i- 1) (snd(x)) ) | x <- q ]
\end{code}



The Minimax algorithm
====
\begin{code}
data Game s m = Game {
    startState    :: s,
    showGame      :: s -> String,
    move          :: Player -> s -> m -> (Player,s),
    moves         :: Player -> s -> [m],
    value         :: Player -> s -> Double}

kalahaGame :: Kalaha -> Game KState KPos
kalahaGame k = Game {
    startState = startStateImpl k,
    showGame   = showGameImpl k,
    move       = moveImpl k,
    moves      = movesImpl k,
    value      = const (valueImpl k)}

startTree :: Game s m -> Player -> Tree m (Player,Double) 
startTree g p = tree g (p, startState g)
    
\end{code}
The function `tree`
----

    
\begin{code}
tree      :: Game s m -> (Player, s) -> Tree m (Player, Double)
tree = undefined
    
\end{code}
The function `minimax`
----

    
\begin{code}
minimax   :: Tree m (Player, Double) -> (Maybe m, Double)
minimax = undefined
\end{code}

The function `minimaxAlphaBeta`
----

\begin{code}
type AlphaBeta = (Double,Double)

minimaxAlphaBeta :: AlphaBeta -> Tree m (Player, Double) -> (Maybe m, Double)
minimaxAlphaBeta = undefined
\end{code}

Testing and sample executions
====
