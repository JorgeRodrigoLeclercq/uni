def bellman_equation(action, state, number_of_elements, probabilities, bellman):
    """ Sum for the bellman equation. """
    total = 0
    for i in range(number_of_elements):
        x = float(probabilities[action][state][i])
        y = bellman[i]
        total = (x * y) + total
    return total


def end(boolean_list):
    """ Verifies if all states have reached a final bellman value. """
    for i in boolean_list:
        if not i:
            return False
    return True


def print_result(bellman, policy):
    """ Prints the results. """
    aux = 0
    for i in range(len(bellman)):
        print("State " + str(i + (16 - aux)) + " degrees has a final cost of " + str(bellman[i]) +
              " following the optimal policy of " + str(policy[i]))
        aux += 0.5


def mdp_thermostat(file_on, file_off, turn_on, turn_off, difference):
    """ Markov Decision Process for reaching 22 degrees with a thermostat. """
    probabilities_on = []  # probabilities when turning on
    probabilities_off = []  # probabilities when turning off

    # Getting the values of the excel in the arrays
    file = open(file_on, "r")
    for row in file:
        probabilities_on.append(row.split(','))

    file = open(file_off, "r")
    for row in file:
        probabilities_off.append(row.split(','))

    probabilities = [probabilities_on, probabilities_off]
    # probabilities[0] = matrix of probabilities when turning on
    # probabilities[1] = matrix of probabilities when turning off

    number_of_elements = len(probabilities[0])  # could be also probabilities[1], as matrix are square and congruent

    bellman = [0 for i in range(number_of_elements)]  # array with the values of the bellman equation
    final_bellman = [False for i in range(number_of_elements)]  # array for knowing if the state is finished
    policy = ['' for i in range(number_of_elements)]  # array with the policy of each state

    # As the bellman equation of 22 will be 0 always
    final_bellman[(22 - 16) * 2] = True
    policy[(22 - 16) * 2] = "THIS IS THE FINAL STATE"

    # Loop while we haven't finished all states
    while not end(final_bellman):

        # Auxiliary array for each cycle of bellman
        new_bellman = [0 for i in range(number_of_elements)]

        for i in range(number_of_elements):
            # New cycle for all the states

            if not final_bellman[i]:
                # States that haven't finished
                bellman_on = turn_on + bellman_equation(0, i, number_of_elements, probabilities, bellman)
                bellman_off = turn_off + bellman_equation(1, i, number_of_elements, probabilities, bellman)
                new_bellman[i] = min(bellman_on, bellman_off)

                if abs(new_bellman[i] - bellman[i]) <= difference:
                    # If the difference between previous and actual state is lower than DIFFERENCE
                    final_bellman[i] = True

                    # Assigns the policy
                    if bellman_on < bellman_off:
                        policy[i] = "TURN ON"
                    else:
                        policy[i] = "TURN OFF"

            else:

                # If the bellman is final then keep its value
                new_bellman[i] = bellman[i]

        # Substitute the previous bellman with the new one
        bellman = new_bellman.copy()

    # Prints the result
    print_result(bellman, policy)


#################################################################
######################## TESTING ################################
#################################################################
FILE_ON = 'turn_on.csv'
FILE_OFF = 'turn_off.csv'
costs = [[2.25, 1.5], [21.68, 22.68], [1, 1], [2, 0]]
DIFFERENCE = 0.001

for cost in costs:
    print("This are the optimal policies for each state when the costs are "
          + str(cost[0]) + " for turn on and "
          + str(cost[1]) + " for turn off "
                           "and the difference is " + str(DIFFERENCE)
          + (" (and the probabilities come from the " + FILE_ON + " and " + FILE_OFF + " files)"))
    mdp_thermostat(FILE_ON, FILE_OFF, cost[0], cost[1], DIFFERENCE)
    print('\n')

