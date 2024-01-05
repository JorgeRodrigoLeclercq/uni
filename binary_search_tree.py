# from binarysearchtree import BinarySearchTree
from bst import BinarySearchTree
from bst import BinaryNode


class AVLTree(BinarySearchTree):
    """Perform basic operations on a binary search tree while keeping the balance."""

    # Override insert method from base class to keep it as AVL
    def insert(self, elem: object) -> None:
        """Inserts a new node, with key and element elem."""
        self._root = self._insert(self._root, elem)

    def _insert(self, node: BinaryNode, elem: object) -> BinaryNode:
        """Gets a node, searches the place to insert a new node with element e (using super()._insert),  and then,
        the function has to balance the node returned by the function super.()_insert."""
        node = super()._insert(node, elem)
        node = self._rebalance(node)
        return node

    # Override remove method from base class to keep it as AVL
    def remove(self, elem: object) -> None:
        """Removes a node, with key and element elem."""
        self._root = self._remove(self._root, elem)

    def _remove(self, node: BinaryNode, elem: object) -> BinaryNode:
        """Gets a node, searches the node with element elem in the subtree that hangs down node , and
        then remove this node (using super()._remove). After this, the function has to balance the node returned by the
        function super()._remove."""
        node = super()._remove(node, elem)
        node = self._rebalance(node)
        return node

    # BALANCE FACTOR CHECK
    def bf_height(self, node):
        if node is None:
            return 0
        return self._height(node.left) - self._height(node.right)

    # SIMPLE RIGHT ROTATION
    def rot_sr(self, node):
        # If there is more than one node
        new_root = node.left
        node.left = None
        if new_root.right is not None:
            node.left = new_root.right
        new_root.right = node
        return new_root

    # SIMPLE LEFT ROTATION
    def rot_sl(self, node):
        # If there is more than one node
        new_root = node.right
        node.right = None
        if new_root.left is not None:
            node.right = new_root.left
        new_root.left = node
        return new_root

    def _rebalance(self, node: BinaryNode) -> BinaryNode:
        # There is no unbalance
        if -1 <= self.bf_height(node) <= 1:
            return node

        # Unbalanced at left
        if self.bf_height(node) >= 2:
            if not node.left.left:
                # Left-right rotation
                node.left = self.rot_sl(node.left)
            return self.rot_sr(node)

        # Unbalanced at right
        if self.bf_height(node) <= -2:
            if not node.right.right:
                # Right-left rotation
                node.right = self.rot_sr(node.right)
            return self.rot_sl(node)
