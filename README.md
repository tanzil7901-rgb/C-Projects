# Dynamic-array-implementation-in-C

This is my first ever C beginner programming project as i thought it was great to learn and applicable for future use, let me break down the time complexities:

init_list = O(1)
list_add = O(1)
list_remove = O(n)
list_free = O(1)

Pretty fast right?? also, ik the list_add and list_remove functions are weird, 
the list_add() adds items as if the list was a stack... it ONLY pushes new items on the top, 
HOWEVER, list_remove() can remove ANY item in the list.
