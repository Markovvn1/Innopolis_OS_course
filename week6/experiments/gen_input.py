import torch


def generate(N, saturation, quantum):
	"""
	Args:
		N(int): amount of processes
		saturation(float): ratio of (sum of burst time) and (time of working)
			E.g. if saturation == 1, then (sum of burst time) == (time of working)
	"""
	assert isinstance(N, int) and N > 0
	assert saturation > 0 and saturation <= 1

	duration_per_process = (1 / (torch.rand(N) * 0.95 + 0.05)).round().int()
	working_time = duration_per_process.sum() / saturation
	start_time = (torch.rand(N) * (working_time - duration_per_process)).int()
	start_time = torch.cat((torch.zeros(1, dtype=torch.int), start_time[1:]))
	start_time.sort()

	# print(duration_per_process.sum(), (start_time + duration_per_process).max(), working_time)

	res = [(i.item(), j.item()) for i, j in zip(start_time, duration_per_process)]

	with open("input.txt", "w") as f:
		f.write(f"{N}\n")
		f.write("\n".join([f"{i} {j}" for i, j in res]) + "\n")
		f.write(f"{quantum}\n")

generate(500, 1, 2)