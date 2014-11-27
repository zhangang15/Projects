__author__ = 'Anglies'



# three states s1, s2, s3

start_probability = [0.25, 0.5, 0.25]

transition_probability = [[0.4, 0.1, 0.3], [0.5, 0.4, 0.3], [0.1, 0.5, 0.4]]

emission_probability = [[0.4, 0.25, 0.1], [0.4, 0.25, 0.1], [0.1, 0.25, 0.4], [0.1, 0.25, 0.4]]

while True:

    input_sequence_string = raw_input("Please input DNA sequence:\n")

    sequence_string = input_sequence_string.lower()

    alpha = [[None for i in range(3)] for j in range(len(sequence_string))]

    for k in range(0, 3):
        if sequence_string[0] == "a":

            alpha[0][k] = start_probability[k] * emission_probability[0][k]


        elif sequence_string[0] == "c":

            alpha[0][k] = start_probability[k] * emission_probability[1][k]

        elif sequence_string[0] == "t":

            alpha[0][k] = start_probability[k] * emission_probability[2][k]


        elif sequence_string[0] == "g":

            alpha[0][k] = start_probability[k] * emission_probability[3][k]

    i = 0
    flag = None
    # tmp_list = []
    for s in sequence_string[1:]:
        if i + 1 < len(sequence_string):
            # print len(sequence_string)
            if s == "a":
                sum1 = alpha[i][0] * transition_probability[0][0] + alpha[i][1] * transition_probability[0][1] + \
                       alpha[i][2] * transition_probability[0][2]
                tmp1 = emission_probability[0][0] * sum1
                sum2 = alpha[i][0] * transition_probability[1][0] + alpha[i][1] * transition_probability[1][1] + \
                       alpha[i][2] * transition_probability[1][2]
                tmp2 = emission_probability[0][1] * sum2
                sum3 = alpha[i][0] * transition_probability[2][0] + alpha[i][1] * transition_probability[2][1] + \
                       alpha[i][2] * transition_probability[2][2]
                tmp3 = emission_probability[0][2] * sum3

                alpha[i + 1][0] = tmp1 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][1] = tmp2 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][2] = tmp3 / (tmp1 + tmp2 + tmp3)




            elif s == "c":
                sum1 = alpha[i][0] * transition_probability[0][0] + alpha[i][1] * transition_probability[0][1] + \
                       alpha[i][2] * transition_probability[0][2]
                tmp1 = emission_probability[1][0] * sum1
                sum2 = alpha[i][0] * transition_probability[1][0] + alpha[i][1] * transition_probability[1][1] + \
                       alpha[i][2] * transition_probability[1][2]
                tmp2 = emission_probability[1][1] * sum2
                sum3 = alpha[i][0] * transition_probability[2][0] + alpha[i][1] * transition_probability[2][1] + \
                       alpha[i][2] * transition_probability[2][2]
                tmp3 = emission_probability[1][2] * sum3

                alpha[i + 1][0] = tmp1 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][1] = tmp2 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][2] = tmp3 / (tmp1 + tmp2 + tmp3)


            elif s == "t":
                sum1 = alpha[i][0] * transition_probability[0][0] + alpha[i][1] * transition_probability[0][1] + \
                       alpha[i][2] * transition_probability[0][2]
                tmp1 = emission_probability[2][0] * sum1
                sum2 = alpha[i][0] * transition_probability[1][0] + alpha[i][1] * transition_probability[1][1] + \
                       alpha[i][2] * transition_probability[1][2]
                tmp2 = emission_probability[2][1] * sum2
                sum3 = alpha[i][0] * transition_probability[2][0] + alpha[i][1] * transition_probability[2][1] + \
                       alpha[i][2] * transition_probability[2][2]
                tmp3 = emission_probability[2][2] * sum3

                alpha[i + 1][0] = tmp1 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][1] = tmp2 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][2] = tmp3 / (tmp1 + tmp2 + tmp3)

            elif s == "g":
                sum1 = alpha[i][0] * transition_probability[0][0] + alpha[i][1] * transition_probability[0][1] + \
                       alpha[i][2] * transition_probability[0][2]
                tmp1 = emission_probability[3][0] * sum1
                sum2 = alpha[i][0] * transition_probability[1][0] + alpha[i][1] * transition_probability[1][1] + \
                       alpha[i][2] * transition_probability[1][2]
                tmp2 = emission_probability[3][1] * sum2
                sum3 = alpha[i][0] * transition_probability[2][0] + alpha[i][1] * transition_probability[2][1] + \
                       alpha[i][2] * transition_probability[2][2]
                tmp3 = emission_probability[3][2] * sum3

                alpha[i + 1][0] = tmp1 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][1] = tmp2 / (tmp1 + tmp2 + tmp3)
                alpha[i + 1][2] = tmp3 / (tmp1 + tmp2 + tmp3)

            else:
                print "Invalid input, please input again."
                flag = True
                break

        i += 1
        if flag != True:
            print "The alpha matrix is: \n ", alpha, "\n"

            prob_s1 = alpha[i - 1][0] / (alpha[i - 1][0] + alpha[i - 1][1] + alpha[i - 1][2])
            prob_s2 = alpha[i - 1][1] / (alpha[i - 1][0] + alpha[i - 1][1] + alpha[i - 1][2])
            prob_s3 = alpha[i - 1][2] / (alpha[i - 1][0] + alpha[i - 1][1] + alpha[i - 1][2])

            print "The probability of s1 is: ", prob_s1
            print "The probability of s2 is: ", prob_s2
            print "The probability of s3 is: ", prob_s3


