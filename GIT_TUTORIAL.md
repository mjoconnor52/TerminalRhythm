## USING GITHUB

Common commands you may want to remember when using github

### Status of repo
`git status`

Before you want to push or pull anything, you should probably check the status of the repo, which this command will do. Basically, it will allow you to see how many commits behind you are (if any, you need to pull), what branch you're on, and any content you may need to push/commit. 

### Pulling from the repo

`git pull`

When you are working in git and someone has made some changes to the project, it is always a good idea to pull from the repo. Try to make this one of the first commands you do everytime you want to change something. 

**NOTE** If someone has pushed changes to a file that you have modified, try to use `git stash` to stash your current changes, `git pull` to update your context, and finally `git stash pop` to then go into VS code conflict merger and merge the conflicts between the branches. It is can be *quite* difficult to fix if the files have changed significantly. So try to push and pull as often as you can.   


### Staging your content
The first stage of pushing content: 
`git add .`

This command will allow you to stage your changes to then be committed, if you want to reset your changes
or you added a file you did not want to add, please do `git restore .` 

**NOTE** You can also only stage specific files but doing `git add {filename}`, if you only want to stage that one change 

### Committing your content
The second stage of pushing content: 

`git commit -m "{comment}"`

This allows you to commit the files with a comment to the repo, saying they are ready to be pushed into the repo. At some companies or bigger projects, this is where your code gets reviewed by a superivor but we are all friends here so it is just another step in the process. To undo the commit, you must use the command `git reset --soft HEAD^` to return back to the state before the commit. 

### Pushing your content
The third and final stage of pushing your content

`git push`

This will push all your changes into the main repo and make it the new head/node that everyone will pull if the use `git pull`. This means your work has been completed and you want to offically save the changes. I've never had to undo a push, but according to stack overflow, it would be something like this `git push -f origin last_known_good_commit:branch_name`. It's never something you want to end up doing, but if it is needed, it will fix it. 

### Branches

If you want to make some context that may mess with someone else's stuff, you can make a seperate branch with `git checkout -b {branch-name}`. Use `git checkout {branchname}` if to swap to a branch. lmk if you want more info. 

