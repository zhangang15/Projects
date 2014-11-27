import re
import time

__author__ = 'anglies'

import email
import HTMLParser
import os
import sys
import math


def findall(content):
    return re.findall('[a-zA-Z]+', content)


class Naive_Bayes():
    def __init__(self, ham_train_dir=None, spam_train_dir=None, ham_test_dir=None, spam_test_dir=None):
        self.spam_size = 0
        self.ham_size = 0
        self.frequency_dic = {}
        self.s_train = spam_train_dir
        self.h_train = ham_train_dir
        self.h_test = ham_test_dir
        self.reg = re.compile("[^a-zA-Z]")

    def train_spam(self, t_spam_dir):
        filelist = []

        for root, dirs, files in os.walk(t_spam_dir):
            for file in files:
                filelist.append(os.path.join(root, file))
        for filename in filelist:
            spam_wordset = set()
            self.spam_size += 1
            f = open(filename, "r")
            msg = email.message_from_string(f.read().decode('ascii', 'ignore'))
            content = ''
            for part in msg.walk():
                if part.is_multipart():
                    continue
                content_type = part.get_content_type()
                content += part.get_payload()

            string_without_tags = self.strip_tags(content)
            for i in [n.strip() for n in findall(string_without_tags.lower())]:

                if i not in self.frequency_dic:
                    self.frequency_dic[i] = [1, 0]
                else:
                    self.frequency_dic[i][0] += 1
            f.close()

    def train_ham(self, t_ham_dir):
        filelist = []

        for root, dirs, files in os.walk(t_ham_dir):
            for file in files:
                filelist.append(os.path.join(root, file))
        for filename in filelist:
            ham_wordset = set()
            self.ham_size += 1
            f = open(filename, "r")
            msg = email.message_from_string(f.read().decode('ascii', 'ignore'))
            content = ''
            for part in msg.walk():
                if part.is_multipart():
                    continue
                content_type = part.get_content_type()
                content += part.get_payload()

            string_without_tags = self.strip_tags(content)
            for i in [n.strip() for n in findall(string_without_tags.lower())]:

                if i not in self.frequency_dic:
                    self.frequency_dic[i] = [0, 1]
                else:
                    self.frequency_dic[i][1] += 1
            f.close()

    def strip_tags(self, html):
        from HTMLParser import HTMLParser

        html = html.strip()
        html = html.strip("\n")
        result = []
        parse = HTMLParser()
        parse.handle_data = result.append
        parse.feed(html)
        parse.close()
        return "".join(result)

    def classify(self, unknown_dir):
        filelist = []

        spam_counter = 0
        total_size = 0
        for root, dirs, files in os.walk(unknown_dir):
            for file in files:
                filelist.append(os.path.join(root, file))

        for filename in filelist:
            check_wordset = set()
            word_prob_list = []
            w_list = []
            total_size += 1
            f = open(filename, "r")
            msg = email.message_from_string(f.read().decode('ascii', 'ignore'))
            content = ""
            for part in msg.walk():
                if part.is_multipart():
                    continue
                content_type = part.get_content_type()
                content += part.get_payload()

            string_without_tags = self.strip_tags(content)
            t2 = time.time()

            for i in [n.strip() for n in findall(string_without_tags.lower())]:
                check_wordset.add(i)
            for word in check_wordset:
                if len(word) > 3 and len(word) < 15:
                    if word in self.frequency_dic:
                        if self.frequency_dic[word][0] == 0:
                            s_prob = 0.001
                        else:
                            s_prob = float(self.frequency_dic[word][0]) / self.spam_size
                        if self.frequency_dic[word][1] == 0:
                            h_prob = 0.001
                        else:
                            h_prob = float(self.frequency_dic[word][1]) / self.ham_size
                        p_s = 1.0 * self.spam_size / (self.ham_size + self.spam_size)
                        p_h = 1.0 * self.ham_size / (self.ham_size + self.spam_size)
                        prob = float((s_prob) * p_s) / (((p_s * s_prob) + (p_h * h_prob)))
                        prob_tuple = (word, prob, s_prob, h_prob)
                        word_prob_list.append(prob_tuple)
                    else:
                        t = (word, 0.4, 0.001, 0.001)
                        word_prob_list.append(t)
            word_prob_list.sort(key=lambda word_pro: word_pro[1], reverse=True)
            f.close()

            i = 1
            down_right = 1.0
            upper = 1.0
            while i < len(word_prob_list) and i < 300:
                list = []
                upper = word_prob_list[i][1] * upper

                down_right = float(1.0 - word_prob_list[i][1]) * down_right

                i += 1
            probability = float(upper) / (upper + down_right)

            if probability > 0.88:
                spam_counter += 1

        percent = float(spam_counter) / total_size

        return percent


if __name__ == '__main__':
    nb = Naive_Bayes(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    ham_train = sys.argv[1].split("=")[1]
    spam_train = sys.argv[2].split("=")[1]
    ham_test = sys.argv[3].split("=")[1]
    spam_test = sys.argv[4].split("=")[1]

    nb.train_ham(ham_train)
    nb.train_spam(spam_train)
    p1 = nb.classify(ham_test)

    p2 = nb.classify(spam_test)
    print "Size of ham training set: " + str(nb.ham_size) + "emails"
    print "Size of spam training set: " + str(nb.spam_size) + "emails"
    total_no = nb.ham_size + nb.spam_size
    h_p = (1 - p1) * 100
    s_p = p2 * 100
    print "Percentage of ham classified correctly: " + ('%.2f%%' % h_p)
    print "Percentage of spam classified correctly: " + ('%.2f%%' % s_p)
    total = (nb.ham_size * (1.0 - p1)) + (nb.spam_size * p2)
    t = (total / total_no) * 100
    print "Total accuracy: " + ('%.2f%%' % t)
    accuracy = math.fabs((p1 - p2)) * 100
    print "False Positives: " + ('%.2f%%' % (p1 * 100))

