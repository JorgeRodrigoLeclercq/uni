from slist import SNode
from slist import SList


class SList2(SList):

    def sum_lastN(self, n):
        node, result = self._head, 0

        if n < 0:
            # Don't need to add any element
            return None

        else:
            if n > self._size:
                # Add all the elements
                n = self._size

            else:
                # Look for the last node that is added
                for i in range(self._size - n):
                    node = node.next

            # Add all the nodes
            for i in range(n):
                result += node.elem
                node = node.next
        return result

    def insert_middle(self, elem):
        new_node = SNode(elem)

        if self._size != 0:
            node = self._head
            prev = node

            # Compute the middle position
            if self._size % 2 == 0:
                middle = self._size // 2
            else:
                middle = int((self._size + 1) / 2)

            # Reach the middle position
            for i in range(middle):
                prev = node
                node = node.next

            # Change the links of the previous and next nodes
            prev.next, new_node.next = new_node, node

        else:
            # Empty list
            self._head, new_node.next = new_node, None

    def insert_list(self, input_list, start, end):
        if 0 <= start <= end < len(self):
            # Head node of the list
            head_node = self._head

            # Tail node of the list
            last_node_input = input_list._head

            while last_node_input.next:
                last_node_input = last_node_input.next

            if end == 0:
                # First node is deleted
                last_node_input.next, self._head = self._head.next, input_list._head

            else:
                # Rest of cases
                node = head_node
                prev = node

                for i in range(start):
                    prev = node
                    node = node.next
                end_node = node

                if start == 0:
                    self._head = input_list._head
                else:
                    prev.next = input_list._head

                for i in range(end - start):
                    end_node = end_node.next

                last_node_input.next = end_node.next

    def reverseK(self, k):
        # Only valid values of k
        if k > 1:
            elements = self._size
            first_next_group = self._head
            aux = SList()

            if k > self._size:
                k = self._size

            # Reverse of the first k elements in an aux list
            for i in range(k):
                aux.addFirst(first_next_group.elem)

                if i == 0:
                    last_aux = aux._head

                first_next_group = first_next_group.next

            if k == self._size:
                self._head = aux._head  # exception for the first group

            else:
                count = 0

                # While there are enough elements to form groups of k
                while elements >= k:
                    # Exception for the first group
                    if elements == self._size:
                        last_aux.next = first_next_group
                        self._head = aux._head
                        elements -= k
                        aux = SList()

                    # Makes the aux list for the next group
                    elif count < k:
                        aux.addFirst(first_next_group.elem)
                        if count == 0:
                            last_aux2 = aux._head
                        first_next_group = first_next_group.next
                        count += 1  # add to the counter until the list is finished

                    # Changes the list according to the aux one
                    else:
                        last_aux.next = aux._head
                        last_aux2.next = first_next_group

                        # Restarts values for the next group
                        last_aux = last_aux2
                        count = 0
                        aux = SList()
                        elements -= k

                # When there aren't enough elements for another group, it adds the last aux list to the current list
                if elements > 1:
                    for i in range(elements):
                        aux.addFirst(first_next_group.elem)
                        first_next_group = first_next_group.next
                        elements -= 1
                        last_aux.next = aux._head

    def maximum_pair(self):
        if self._size == 0:
            return None

        elif self._size == 1:
            return self._head.elem

        else:
            # Creates an inverted list
            node, l = self._head, SList()
            i, aux = 0, 0

            while i <= self._size:
                if i == self._size - (aux + 1):
                    if i == self._size - 1:
                        # For the first time
                        l._head = SNode(node.elem)
                        node2 = l._head

                    else:
                        # For the rest of the cases
                        node2.next = SNode(node.elem)
                        node2 = node2.next
                    i, node = 0, self._head
                    aux += 1

                else:
                    node = node.next
                    i += 1
                if aux == self._size:
                    i = self._size + 1

            # Finding if it is odd or even
            odd = 0
            if self._size % 2 != 0:
                odd = 1

            # Comparing the values from self_list and l
            node, node2 = self._head, l._head
            for i in range((self._size + odd) // 2):
                if odd and i == ((self._size + odd) // 2) - 1:
                    # Exception for odd lists
                    aux2 = node.elem

                else:
                    aux2 = node.elem + node2.elem

                if i == 0:
                    # Initialize result
                    result = aux2

                elif result < aux2:
                    result = aux2
                node, node2 = node.next, node2.next

        return result