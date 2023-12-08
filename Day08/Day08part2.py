   
def combine_phased_rotations(a_period, a_phase, b_period, b_phase):
    """Combine two phased rotations into a single phased rotation

    Returns: combined_period, combined_phase

    The combined rotation is at its reference point if and only if both a and b
    are at their reference points.
    """
    gcd, s, t = extended_gcd(a_period, b_period)
    phase_difference = a_phase - b_phase
    pd_mult, pd_remainder = divmod(phase_difference, gcd)
    if pd_remainder:
        raise ValueError("Rotation reference points never synchronize.")

    combined_period = a_period // gcd * b_period
    combined_phase = (a_phase - s * pd_mult * a_period) % combined_period
    return combined_period, combined_phase


def arrow_alignment(red_len, green_len, advantage):
    """Where the arrows first align, where green starts shifted by advantage"""
    period, phase = combine_phased_rotations(
        red_len, 0, green_len, -advantage % green_len
    )
    return -phase % period


def extended_gcd(a, b):
    """Extended Greatest Common Divisor Algorithm

    Returns:
        gcd: The greatest common divisor of a and b.
        s, t: Coefficients such that s*a + t*b = gcd

    Reference:
        https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Pseudocode
    """
    old_r, r = a, b
    old_s, s = 1, 0
    old_t, t = 0, 1
    while r:
        quotient, remainder = divmod(old_r, r)
        old_r, r = r, remainder
        old_s, s = s, old_s - quotient * s
        old_t, t = t, old_t - quotient * t

    return old_r, old_s, old_t


cycles = [21049, 14363, 15989, 16531, 19241, 19783]
#cycles = [21049, 267921900971]
steps = [28159343336, 28159322198, 28159331141, 28159335206, 28159338187, 28159339813]
#steps = [28159343336, 267921900971]
offsets = [12185, 0, 0, 0, 0, 0]

for i in range(len(cycles)):    
    offsets[i] = steps[i] % cycles[i]
    print(f"cycles {cycles[i]} steps {steps[i]} offset {offsets[i]} ")

    
for i in range(1,len(cycles)):
    offset = offsets[i] - offsets[i-1]
    print(f"offset {offset}")
    if offset >= 0:
        print(arrow_alignment(red_len=cycles[i-1], green_len=cycles[i], advantage=offset))
    else:
        print(arrow_alignment(red_len=cycles[i], green_len=cycles[i-1], advantage=-offset))