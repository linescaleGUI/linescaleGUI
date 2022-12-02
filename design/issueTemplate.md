# Description
All created issues on GitHub must be formatted according to the definitions of
this file. There are 3 different project management labels which can be assigned
to each issue. Those issues must have a comment with following information.
Regarding the "Tracked by" comment. Currently, the automatically created
"tracked by" tag on Github is only visible if the issue is opened within the
"Issue" tab on GitHub. If the issue is inspected from any projects panel, this
tag is not visible. Therefore, the comment must be added.

# 01-Epics
    - [ ] <02-UserStory 1>
    - [ ] <02-UserStory 2>

# 02-UserStory
    ### Tracked by #<Epic issue number>
    <Short description of the UserStory>

    - [ ] <02-Task 1>
    - [ ] <02-Task 2>

# 03-Task
A short description of the task is necessary. Each task must have a definition
of done (DoD) assigned to it. If applicable the "Implement", "Tests" and
"Documentation" definitions can be added.

    ### Tracked by #<UserStory issue number>
    <Short description of the task>

    ### DoD
    - [ ] Implementation
    - [ ] Tests
    - [ ] Documentation
    - [ ] <Other definitions>
