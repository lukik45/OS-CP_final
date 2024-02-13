# OS-CP_final
> This repo contains the final project on **Operating Systems & Concurrent Progrramming**

My project consists of only a single program, which as I believe satisfies both conditons:
1. Visiting a museum at the same time by as many people as possible,
2. Leaving the museum by people visiting hall B in the shortest possible time.
---
## Conceptions:
- Visitors as threads, (threads can loop multiple times to simulate new visitors).
- There is no race condition on any resources since multiple visitors can contemplate the same pieces of art at the same time.
- Time of visiting a given hall is not predefined. (Algorithm draws random number from some interval every time a visitor enters the room).
---
## Scheme:
(1) Each visitor initially waits for some time (in real life all visitors would not
    want to enter the museum at exactly the same time)

(2) Then visitor decides how much is he willing to spend in hall A

(3) Visitor decides whether he wants to enter hall B or leave the museum
  - (3a) if he chooses to enter hall B, then he decides how long he wants to stay
  - (4a) visitor indicates that he wants to leave hall B, if current state allows such
        operation then he leaves, otherwise he waits for free space in hall A, and then go
        back to hall A
 
(5) If B was visited, then comes back to hall A, spends some time looking at arts once again and finally leaves.

If visitor have chosen not to visit hall B, then leaves hall A immediately after looking at art.
> more detailed expanations of the algorithm are included as comments in the code.


---
## Some insights and remarks:
- I can't allow visitors to enter hall A from outside when there is only one place left in hallA because I don't know in advance whether the visitor is willing to enter hall B or not
  > this is very important because if I let to a situation that hall B is full and hall A is full and all processes from hall A want to enter hallB, then I would have the **deadlock**
  
  > to prevent such situation, I decided to use mutexes and conditional variables instead of counting semaphores, even though semaphores seemed to be more intuitive at a first glance.

- Mutexes lock `countA` and `countB` variables, whose role is to count number of people in respective halls.
- Conditional variables play crucial role in the synchronisation of my algorithm
  > They are used to perform `wait` and `broadcast` calls, to communicate between threads

- mutexes are always nested
  > to avoid the deadlock

---
## About variables known at compile time

- `THREAD_NUM` = 25: number of running threads (not equal to the total number of visitors 
- `NA` = 6: limit for hall A
- `NB` = 4: limit for hall B
- `PROB_OF_B` = 0.7: the probability that visitor will decide to visit hall B

#### Variables not related with synchronisation problem:
- `PROB_OF_ENTERING_AGAIN` = 0.75: the probability, that given thread won't terminate itself, but will simulate another visitor
                                
- `TIME_MIN` = 30000: minimal time of visiting given hall
- `TIME_MAX` = 100000: maximal time of visiting given hall
  > these variables help to ilustrate the performance of an algorithm

## Evaluation
- to evaluate the code, just compile it with `make` command
- then, run `./museum`
- to evaluate the performance under different initial values, you can modify the above variables in `headers.h` file.

