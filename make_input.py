import random

def make_rule(fp, size):
    rows = (size - 1) * (size - 1)
    for i in range(rows):
        rule_string = ''.join([ random.choice([ 'U', 'D', 'L' ]) for x in range(27) ])
        print(rule_string, file = fp)

def make_days(fp, size, days):
    left_and_top = size * 2 - 1
    for i in range(days):
        zero_segment = random.randrange(left_and_top)
        one_segment = random.randrange(left_and_top - zero_segment)
        two_segment = left_and_top - zero_segment - one_segment
        print('{} {} {}'.format(zero_segment, one_segment, two_segment),
              file = fp)

def make_test(filename, size, days):
    with open(filename, "w") as fp:
        print('{} {}'.format(size, days), file = fp)
        make_rule(fp, size)
        make_days(fp, size, days)
        
def main():
    random.seed()
    make_test("input4.txt", 100, 1)
    make_test('input5.txt', 200, 1)
    make_test('input6.txt', 500, 1)
    make_test('input7.txt', 1000, 1)
    make_test('input8.txt', 3000, 1)
    make_test('input9.txt', 1000, 10)
    make_test('input10.txt', 1000, 20)
    make_test('input11.txt', 1000, 100)
    make_test('input12.txt', 1000, 1000)

if __name__ == '__main__':
    main()
