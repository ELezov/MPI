#!/bin/bash
#SBATCH --job-name=JobName
#SBATCH --output=JobName-%j.out
#SBATCH --error=JobName-%j.err
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2

MACHINEFILE="nodes.$SLURM_JOBID"
srun -l /bin/hostname | sort -n | awk '{print $2}' > $MACHINEFILE

mpiexec.hydra --bootstrap slurm -f $MACHINEFILE -ppn $SLURM_NTASKS_PER_NODE -genv I_MPI_FABRICS=shm:tcp ./a.out

rm $MACHINEFILE

