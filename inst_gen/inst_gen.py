# -*- coding:utf-8 -*-

import random
import os.path

class InstGen(object):
	__n_mach = 0
	__n_jobs = 0

	def __init__(self, machines, jobs):
		self.__n_mach = machines
		self.__n_jobs = jobs
	
	def generate(self, path = None): 
		if path != None :
			pathName = path
		else :
			pathName = "../../../instances_JSP/"

		arqName = pathName+"pc_"+str(self.__n_mach)+""+str(self.__n_jobs)+".jsp"
		arq = open(arqName, 'w')
		
		""" Conteúdo da instância """
		content = []
		content.append(str(self.__n_jobs)+' ')
		content.append(str(self.__n_mach)+'\n')
		
		for i in range(self.__n_jobs):
			for j  in range(self.__n_mach):
				r = random.randint(0, self.__n_mach-1)

				content.append(str(r)+' ')
				content.append(str(random.randint(20, 99))+' ')
			
			content.append('\n')

		arq.writelines(content)
		arq.close()
		
		print "Arquivo gerado com sucesso!"
		print " Número de trabalhos: " + str(self.__n_jobs)
		print " Número de máquinas: " + str(self.__n_mach)
		print " O arquivo está em: " + pathName





if __name__ == '__main__':
	print "  ---- Gerador de instâncias para o problema Jobshop ---- "
	print " |     Autor: Pedro Coelho                               |"
	print " |     Criado em: 16 de julho                            |"
	print "  ------------------------------------------------------- "

	jobs = input("Digite o número de trabalhos: ")
	mach = input("Agora, digite o número de máquinas: ")
	
	nInstances = input("Ok, mas serão quantas instâncias? ")

	inst = InstGen(mach, jobs)
	for i in range(nInstances):
		inst.generate()

	











		
	



