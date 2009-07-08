The rsvp module allows users to create invitations to events and invite people to this event and maintain a 
list of guests.Starting with the drupal 6 integration, the rsvp module provides invitations for events 
created by the event module or the date module, but not mutual.

If you use RSVP I like you to support my work on RSVP for Drupal 6. Please consider 
donating to my PayPal account. A reference can be found in the project page: 
http://drupal.org/node/350019


RSVP 2 comes with many exciting new features:
extensible invitation theme engine, extensible stylesheets, iconsets, images, backgrounds, 
emails are configurable (template based), block support, 
creating invitations now provides expert and basic mode, 
guests can store their real name in the system, actions, 
open invitations (for users only), maximum guest limitation, start end end reply date, 
notifications for guests and moderators, co moderator support.

Requirements:
-Event or Date module.

Optional modules:
-Simplenews
-Buddylist2
-Friendlist
-OrganicGroups
-Signup
-UserRelationships

Installation
---------------------------
   1. Copy the rsvp directory into your drupal modules directory.
   2. Go to Administer | Site Building | Modules and
         1. enable the RSVP module.
         2. enable either the 'RSVP date connector' or the 'RSVP event connector' module. Do not enable both at the same time. If you decide to switch from one connector to the other you have to use the uninstall option to uninstall the current connector before you
            enable the other one. Be careful. Uninstalling a connector will remove all existing invitation and RSVP records. Disabling and enabling is fine though and will not remove existing invitations.
         3. choose which RSVP plugins you want to enable. This decision is mostly based on which of those modules you use. Except for
            the signup plugin, all others will allow the creator of an invitation to add guests based on the module it stands for.
            I personally would enable the RSVP roles plugin because it allows to select guests by drupal system role, and the plaxo plugin which allows you to import addresses from your external addressbook.



Configuration
------------------------------
   1. Enable permissions appropriate to your site.
      The rsvp main module provides the following permissions:

    * administer rsvp - full access.
    * maintain rsvp - permission to modify all invitations, but no permissions to modify 
      the general rsvp settings.
    * rsvp on events - create invitations on nodes.
    * rsvp on own events - create invitations on your own nodes.
    * rsvp system users - permission to invite persons based on available roles.
    * rsvp multiple invitations per event - permission to create more then one invitation per event.

    Additionaly Each plugin has its own permission that needs to be configured. 
    For example the 'rsvp invite simplenews' permission determines who can add guests based on 
    newsletter subscribers.
    
   2. General RSVP setup at 'Administer|Site configuration|RSVP settings'.

    1. General settings
       1. Select the email address that should be used whenever an email will be sent by the 
          RSVP module (invitations, messages to guests).
       2. The Email Options section allows you to modify the email templates that will be used 
          when the RSVP module sends emails to
          guests..
    2. Default Settings
       Specify the initial settings for all invitations that will be created on your website.
       1. Select default theme, style and iconset. Users can create their own themes/stylesheets 
          which will be explained later.
       2. Maximum guests allowed. If the number of guests exceeds the selected maximum, 
          following invitees can not reply with "yes" anymore.
    3. Associations
          - If you use the event module ('RSVP event connector' module is installed and enabled) 
            you are set to create invitations for existing events.
            It is not necessary to create an association for the event module because the event 
            content type uses a build-in field using as event startdate. 
          - If you use the date module ('RSVP date connector' module is installed and enabled) 
            the administrator need to configure for all content types that carry Date or Datetime fields, 
            which field should be used when invitations are being created for that content type. 
            The RSVP module can manage multiple "event enabled" content types. 
            However it is not possible to select more then one field per content type.
               1. Create fields
                  A custom field of type Date or Datetime is required for content types you intend to 
                  create invitations for later. If not done yet create the fields now. 
                  Add a custom field of type Date or Datetime to all the content types that you want 
                  to utilize as potential events in the future.
               2. Create association(s)
                  Select the content type under admin/content/types and find the "RSVP Settings" section.
                  Select the field that you want to use.
                  If the field selection box is empty, you did not setup a proper field inside the content type.

   3. Enable Blocks.
      RSVP supports now three blocks that can be enabled.
      (Host toolbox block, Guest toolbox block and Guest list block).
   
   4. Disable the WYSIWYG editor for the "guests you like to invite" / "Add guests" textarea which 
   is part of the "manage guests" form when creating an invitation. 
   The Wysiwyg editor should be disabled for those fields by default(Tested with FcK). If the editor is visible,
   Please exclude field "edit-recipients" in the editors settings manually.



Manage invitations
------------------------------------
Administrators and Maintainers have one central place where all available invitations 
for all events and all users are listed. It can be found under 
'Adminstrator|Content management|RSVP management'




Create invitations
-------------------------------------
Go to the node you want to create an invitation for and click on the Invitation tab.
If the Invitation tab is not available,
-verify that this content type is enabled in the rsvp settings.
-verify that the user has the right permissions ("rsvp on events" or "rsvp on own events")

After setting up and configuring the invitation the creator has to add guests to the 
guest list for this invitation within the "Manage guests" tab.
The default procedure to add registered users or other persons would be to add them 
to the "Add guests" text field. If the user has the permissions and installed the respective plugin 
he also has the option to select users by role, or by newsgroup,....
In general, when you add a person to the guest list by its email address, 
the RSVP module tries to match the entered email address with a registered users. 
When you add a person to the guest list by its name, the RSVP module tries to match the entered name 
with a registered user name.
The rsvp modules does not allow duplicates in the guest list. 
Adding invitees to the guest list does not automatically send out invitation emails to those invitees. 
That needs to be done in a seperate step. Inside the Guest list there is a section "No invitation
sent" with a list of invitees that have not been notified by email.
Press the "Send invitation" link to send the invitation to those users by email.

Modify invitations
----------------------------------
It is possible to change all setting of an invitation even if it is already underway.
Automatic startdate adjustment is not supported yet. If the startdate of an event/node changes, 
the startdate of the related invitation needs to be changed as well.

Theme enhancements.
----------------------------------
Administrators now can create their own RSVP templates, stylesheets and iconsets that can be used 
to display future invitations. You either can add stylesheets to an existing template to support 
a different color scheme or styles. If that is not enough you can add your own templates to rsvp.
RSVP comes with one build in theme and stylesheet which is called default and is located 
in subfolder modules\rsvp\themes\default and you can use those files as template for your own styles.

User defined templates can be created in the files directory (usually located at sites\default\files\)
-To create your own template with name "nexus", create a folder sites\default\files\rsvp\temes\nexus, 
then copy all files from modules\rsvp\themes\default into the nexus folder. 
The folder name becomes automatically the template name. The name of the stylesheet file without 
the extension is the style that is being displayed when creating an rsvp.

-To create an additional stylesheet "dream" for your nexus template, copy one of the existing .css files 
from folder sites\default\files\rsvp\temes\nexus and rename it to dream.css. Then change the content 
of dream.css to your likings.

-To create your own iconset, with name "moon", create a folder sites\default\files\rsvp\icons\moon, 
then copy all the icon files from modules\rsvp\icons\flags into the newly created moon subfolder. The
folder name is automatically the iconset name. Now you can overwrite all icons in the moon subfolder 
with your own icons.

-To add your own backgrounds, create a folder sites\default\files\rsvp\backgrounds, 
then copy your images into the folder that you want use as background for your invitations.

-To add your own images, create a folder sites\default\files\rsvp\images, then copy your images 
into the folder that you want use as images that are being displayed with your invitation.

Terminology:
-----------------------
    * RSVP:The module name. 
      (The term R.S.V.P. comes from the French expression "répondez s'il vous plaît", meaning "please respond").
    * Event:A node on which an invitation can be created on.
    * Invitation:The invitation generated for an event or an invitation someone receives for an event. 
      A displayed invitation consists of three logical parts. The invitation text, 
      the guest list and the reply box.
    * Invitee:A person invited to an event by adding the person to an invitation.
    * Respondent:An invitee who replied if he will attend or not.
    * Attendee:A respondent who confirmed that he will attend the event.
    * Association:A connection between a content type and a Date and Datetime field created for the content type.
    * Guest list:All persons (= all Invitees) that have been invited to an event.
    * Guest:A person invited to an event (=Invitee).



   
TODO/Features for following releases:
--------------------------------------
- Tighter Integration to event nodes. Why do we need an extra Invitation page at all?? (LOW PRIO)

- more options about who can send messages to whom: checkboxes per group (invitee, respondent, attendees) (LOW PRIO)

- http://drupal.org/node/180153 : Add invitations to "MyCalendar" 
- http://drupal.org/node/219555 : Include links to reply directly from e-mail?



- Integration with CiviCRM 
-http://drupal.org/node/38918, http://drupal.org/node/30522: CiviCRM integration (in all the obvious ways: Personal information being added to Contacts, direct registration, events being added to "Activities")

- Email: http://drupal.org/node/84918: Send emails via privatemsg instead by email.
(send_privatemsg)

- Guest List: Add icon for regular invitees to view status (LOW PRIO).
- Add red reply button instead of link like the event manager module has(-http://em-demo.mind-sky.com/?q=node/3). (LOW PRIO)

- preview option when writing an rsvp
- automatic update of invitations startdate when startdate in node changes. (MORE COMPLICATED)

- Allow parallel usage of event connector and date connector
- allow parameters for views

- http://drupal.org/node/64208 evite reply box


- offer open registration for anonymous users.
- http://drupal.org/node/397888 : For guests, Invitation tab can forward to the invitation if only one invitation is available.
- http://drupal.org/node/397888 : Configure, which options are listed on the basic screen and which are extended.


- Send automatic notification email before the event starts (cron or manual).
- Send automatic invitation email (cron)

- http://drupal.org/node/461720 : Implement uebercart integration instead of reply box .

- optimization of RSVP queries (links, permission queries)






